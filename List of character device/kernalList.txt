/* Implementation of kernel list 
 *
 * Copyright (C) 2016, 2017  Krzysztof Mazur <krzysiek@podlesie.net>
 *
 * Copyright (C) 2018  Monika Targosz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#define pr_fmt(fmt) "ringdev: " fmt
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/wait.h>
/*
 * po¿±dany numer minor urz±dzenia znakowego ringdev,
 * MISC_DYNAMIC_MINOR dla automatycznego wyboru
 */
#define RINGDEV_MINOR   MISC_DYNAMIC_MINOR
#define MAX_SIZE 4096
/*
 * mutex u¿ywany do synchronizacji dostêpu do wszystkich zmiennych modu³u
 */
static struct mutex ringdev_lock;
struct DaneLista{
int i;
struct list_head link;
char *dane;
};

struct x{
int j;//bajt
struct list_head *kursor;//wskaźnik na list haead, typ elementu
};

LIST_HEAD(glowa_listy);

static DECLARE_WAIT_QUEUE_HEAD(kolejka);

static int ringdev_open(struct inode *inode, struct file *filp){
            struct x *x;
            x=kmalloc(sizeof(*x), GFP_KERNEL);
            filp->private_data=x;
            if(x==NULL){
                        return -ENOMEM;
            }
            x->j=0;
            x->kursor=&glowa_listy;
            return 0;
}

static ssize_t ringdev_read(struct file *filp, char __user *buf, size_t count, loff_t *off){
            struct x *x = filp->private_data; //stan;
            ssize_t ret = 0;
            struct DaneLista *ele;
           
            mutex_lock(&ringdev_lock);
            retry:
            if(x->kursor==&glowa_listy)
            x->kursor=x->kursor->next;

            if(x->kursor==&glowa_listy){
                        mutex_unlock(&ringdev_lock);
                        if(wait_event_interruptible(kolejka, !list_empty(&glowa_listy)))
                                    return -ERESTARTSYS;
                                    mutex_lock(&ringdev_lock);
                                    goto retry;
            }
            ele=list_entry(x->kursor, struct DaneLista, link);
            if(ele->i==x->j){
                        if(x->kursor->next==&glowa_listy){
                        mutex_unlock(&ringdev_lock);
                                    if(wait_event_interruptible(kolejka,x->kursor->next!=&glowa_listy))
                                    return -ERESTARTSYS;
                                    mutex_lock(&ringdev_lock);
                         }
                         else{
                                    x->kursor=x->kursor->next;
                                    x->j=0;
                        }
            goto retry;
            }


            if(count > ele->i - x->j)
            count = ele->i - x->j;
            
            ret = -EFAULT;
            if (copy_to_user(buf,ele->dane + x->j, count))
                    goto out_unlock;
            ret = count;
            x->j+= count;

            out_unlock:
            mutex_unlock(&ringdev_lock);
            return ret;
}


static ssize_t ringdev_write(struct file *filp, const char __user *buf,
                    size_t count, loff_t *off){
            ssize_t ret;
            struct DaneLista *ele;
            ele=kmalloc(sizeof(struct DaneLista), GFP_KERNEL);
            if(!ele){
                    return -ENOMEM;
                    }
            ele->dane=kmalloc(count, GFP_KERNEL);
            ele->i=0;
            ele->i=count;
            mutex_lock(&ringdev_lock);
            ret=-EFAULT;
            
            list_add_tail(&ele->link,&glowa_listy);
            if(copy_from_user(ele->dane,buf,count))
                    goto out_unlock;
            ret=count;
            out_unlock:
            mutex_unlock(&ringdev_lock);
            wake_up_interruptible(&kolejka);
            return ret;
}


static int ringdev_release(struct inode *inode, struct file *filp){
            struct x *x = filp->private_data; //stan;
            kfree(x);
            return 0;
}


static const struct file_operations ringdev_fops = {
            .owner = THIS_MODULE,
            .open = ringdev_open,
            .read = ringdev_read,
            .write = ringdev_write,
            .release = ringdev_release,
};


static struct miscdevice ringdev_miscdevice = {
            .minor = RINGDEV_MINOR,
            .name = "ringdev",
            .fops = &ringdev_fops
};


static int __init ringdev_init(void){
            int ret;
            mutex_init(&ringdev_lock);
            ret = misc_register(&ringdev_miscdevice);
            if (ret < 0) {
                    pr_err("can't register miscdevice.\n");
                    return ret;
            }

            pr_info("minor %d\n", ringdev_miscdevice.minor);
            return 0;
}


static void __exit ringdev_exit(void){
            struct DaneLista *ele;
            struct DaneLista *ele2;
            misc_deregister(&ringdev_miscdevice);
            mutex_destroy(&ringdev_lock);
            list_for_each_entry_safe(ele,ele2,&glowa_listy,link){
            kfree(ele->dane);
            kfree(ele);
}


}


module_init(ringdev_init);
module_exit(ringdev_exit);


MODULE_DESCRIPTION("Kernel list device");
MODULE_AUTHOR("Real Name <email@example.com>");
MODULE_LICENSE("GPL");
