// Program ilustrujacy dzialanie struktury na daty

#include <stdio.h>

int main (void)
{
    struct  date    //definicja struktury date mającej trzy zmienne calkowite: month, day, year, 
    {               //tutaj nie jest rezerwowana żadna pamięć
        int  month;
        int  day;
        int  year;
    };

    struct date  today, purchaseDate; //deklaracja zmiennej today i purchaseDate typu struct data 
                                      //powoduje zarezerwowanie pamięci na trzy liczby całkowite wchodzące w skład struktury
    //przypisanie wartości polom struktury
    today.month = 5;                  //ustawienie pola month w zmiennej today na 5
    today.day = 24;
    today.year = 2021;

    printf ("Today's date: %i.%i.%.2i.\n", today.day, today.month, today.year % 100); //wyświetlenie roku jako 21 (reszta z dzielenia)
    //łańcuch formatujący %.2i powoduje, że wyświtlana jest dwucyfrowa liczba całkowita wypełniona z przodu zerami

    return 0;
}
