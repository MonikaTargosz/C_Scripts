I. Podział na pliki 
Co piszemy w plikach .h a co w plikach .c? 
Na początek warto przypomnieć, iż klasa jest typem (podobnie jak struktura czy enum), więc właściwym dla niej miejscem byłby zawsze plik nagłówkowy. Jednocześnie jednak zawiera ona kod swoich funkcji składowych, czyli metod, co czyni ją przynależną do jakiegoś modułu (bo tylko wewnątrz modułów można umieszczać funkcje).
Te dwa przeciwstawne stanowiska sprawiają, że określenie klasy jest najczęściej rozdzielone na dwie części:
definicję, wstawianą w pliku nagłówkowym, w której określamy pola klasy oraz wpisujemy prototypy jej funkcji,
implementację, umieszczaną w module, będącą po prostu kodem wcześniej zdefiniowanych funkcji.
W plikach nagłówkowych z rozszerzeniem .h umieszczamy:
definicje:
stałych,
typów,
struktur (obiektów na zewnątrz),
prototypy funkcji.
Natomiast w pliku o rozszerzeniu .c umieszczamy:
kod,
prywatne (dla biblioteki):
zmienne (static),
funkcje pomocnicze.
#include<plik o tej samej nazwie.h>.
W jaki sposób powinniśmy dzielić kod?

Powinniśmy wydzielić zbiór definicji, zmiennych, funkcji odpowiadających za jedną rzecz czy pojedynczy komponent w programie. Podglądając HALa możemy zobaczyć obsługę peryferiów, handlerów przerwań (oddzielenie funkcjonalności).

Jeżeli bowiem umieściliśmy weń prototypy jakichś funkcji, nieodzowne jest
wpisanie ich kodu w którymś z modułów programu. Zobaczmy zatem, jak należy to
robić.

Przede wszystkim należy udostępnić owemu modułowi definicję struktury, co prawie zawsze
oznacza konieczność dołączenia zawierającego ją pliku nagłówkowego. Jeśli zatem nasza
struktura jest zdefiniowana w pliku struktura.h, to w module kodu musimy umieścić dyrektywę:

#include "klasa.h"

Potem możemy już przystąpić do implementacji funkcji.
Ich kody wprowadzamy w niemal ten sam sposób, który stosujemy dla zwykłych funkcji.
Jedyna różnica tkwi bowiem w nagłówkach tychże funkcji, na przykład:

void funkcja (float fIle)
{
// tutaj kod metody
}

Dalej następuje zwyczajowa lista parametrów i wreszcie zasadnicze ciało metody.
Wewnątrz tego bloku zamieszczamy instrukcje, składające się na kod danej funkcji.


II. Struktury 

Specjalny typ zmiennej mogący przechować wiele wartości. Pudełko na zmienne. Zmienne zawarte w strukturze mogą być różnego typu.

Możemy więc użyć zmiennych do utworzenia nowego, złożonego typu, reprezentującego w całości pojedynczy czujnik BMP280:

struct BMP280_t{
I2C_HandleTypeDef 	*bmp_i2c;
	uint8_t			Address;

	// Coefficients for calculations
	int16_t t2, t3, p2, p3, p4, p5, p6, p7, p8, p9;
	uint16_t t1, p1;
	int32_t t_fine;
};

W ten właśnie sposób zdefiniowaliśmy typ strukturalny. Typy strukturalne (zwane też w skrócie strukturami) to zestawy kilku zmiennych, należących do innych typów, z których każda posiada swoją własną i unikalną nazwę.

Nasz utworzony typ strukturalny składa się zatem z kilku pól struktury, zaś każde z nich
przechowuje jedynie elementarną informację. Zestawione razem reprezentują jednak
złożoną daną o jakimś czujniku.

Zdefiniowane nowego typu o nazwie BMP280_t  jest pozwala na skorzystać z niego tak samo, jak z innych typów w języku C (np. enum’ów). Zadeklarujmy więc przy jego użyciu jakąś przykładową zmienną:

struct BMP280_t Czujnik;

Teraz przydałoby się nadać jej pewną wartość. Jednak, że powyższy Czujnik to tak naprawdę kilka zmiennych w jednym. Niemożliwe jest zatem przypisanie mu zwykłej, „pojedynczej” wartości, właściwej typom skalarnym.

Możemy za to zająć się osobno każdym z jego pól. Cóż zatem zrobić, aby się do nich dostać?...
Skorzystamy ze specjalnego operatora wyłuskania, będącego zwykłą kropką (.).
Pozwala on między innymi na uzyskanie dostępu do określonego pola w strukturze.
Użycie go jest widoczne na poniższym przykładzie:

// wypełnienie struktury danymi
Czujnik.t1 =27;
Czujnik.p1 = 1010;
Czujnik.Address =0x76;

Postawienie kropki po nazwie struktury umożliwia nam niejako „wejście w jej głąb”. Po kropce wprowadzamy więc nazwę pola, do którego chcemy się odwołać.

W przykładzie powyżej czynimy zwykłe przypisanie wartości, lecz równie dobrze mogłoby to być jej odczytanie, użycie w wyrażeniu, przekazanie do funkcji, itp. Nie ma bowiem żadnej praktycznej różnicy w korzystaniu z pola struktury i ze zwykłej zmiennej tego samego typu - oczywiście poza faktem, iż to pierwsze jest tylko częścią większej całości. Co pozwala na zachowanie przejrzystości kodu w rozbudowanych projektach. 

Istnieje również możliwość utworzenia własnego typu danych, którego definicja wygląda następująco:

typedef struct{
I2C_HandleTypeDef 	*bmp_i2c;
	uint8_t			Address;

	// Coefficients for calculations
	int16_t t2, t3, p2, p3, p4, p5, p6, p7, p8, p9;
	uint16_t t1, p1;
	int32_t t_fine;
}BMP280_t;


Natomiast deklaracja zmiennej strukturalnej wygląda : BMP280_t Czujnik;

W tym przypadku, aby przypisać wartość do pola naszej struktury potrzebujemy zmienną a nie typ struktury, co pokazuje przykład poniżej:

BMP280_t.t1=27; (uint16_t = 27)
Czujnik.t1=27;

Struktura jest traktowany jako obiekt w C++ np. przycisk czy przykładowy czujnik.

Istnieje jeszcze jedna droga nadania początkowych wartości polom struktury, którą jest inicjalizacja. Ponieważ podobnie jak w przypadku tablic mamy tutaj do czynienia ze złożonymi zmiennymi, należy tedy posłużyć się odpowiednią formą inicjalizatora podaną poniżej:

// inicjalizacja struktury
BMP280_t Czujnik = { 0x76,27,1010 };

Używamy więc w znajomy sposób nawiasów klamrowych, umieszczając wewnątrz nich
wyrażenia, które mają być przypisane kolejnym polom struktury. Należy przy tym
pamiętać, by zachować taki sam porządek pól, jaki został określony w definicji typu
strukturalnego. Inaczej możemy spodziewać się błędów. Kolejność pól w definicji typu strukturalnego oraz w inicjalizacji należącej doń struktury
musi być identyczna.

Zasadniczą listą będzie po prostu odpowiednia tablica struktur:

const unsigned LICZBA_CZUJNIKÓW = 5;
BMP280_t nCzujnik[LICZBA_CZUJNIKÓW];

Jej elementami staną się dane poszczególnych parametrów należących do czujnika BMP280 zestawione w jednowymiarową tablicę.

Metody obsługi takiej tablicy nie różnią się wiele od porównywalnych sposobów dla tablic
składających się ze „zwykłych” zmiennych. Możemy więc łatwo napisać przykładową,
prostą funkcję, która wyszukuje osobę o danym nicku:

int WyszukajCzujnik(uint8_t Address)
{
// przebiegnięcie po całej tablicy kontaktów przy pomocy pętli for
for (unsigned i = 0; i < LICZBA_CZUJNIKÓW; ++i)
// porównywanie nicku każdej osoby z szukanym
if (nCzujnik[i]. Address ==  Address)
// zwrócenie indeksu pasującego czujnika
return i;
// ewentualnie, jeśli nic nie znaleziono, zwracamy -1
return -1;
}

Zwróćmy w niej szczególną uwagę na wyrażenie, poprzez które pobieramy adresy
kolejnych czujników na naszej liście. Jest nim:
nCzujnik[i]. Address

W zasadzie nie powinno być ono zaskoczeniem. Jak wiemy doskonale, nCzujnik[i]
zwraca nam i-ty element tablicy. U nas jest on strukturą, zatem dostanie się do jej
konkretnego pola wymaga też użycia operatora wyłuskania. Czynimy to i uzyskujemy
ostatecznie oczekiwany rezultat, który porównujemy z poszukiwanym adresem.
W ten sposób przeglądamy naszą tablicę aż do momentu, gdy faktycznie znajdziemy
poszukiwany czujnik. Wtedy też kończymy funkcję i oddajemy indeks znalezionego
elementu jako jej wynik. W przypadku niepowodzenia zwracamy natomiast -1, która to
liczba nie może być indeksem tablicy w C.

Podsumowując, typ strukturalny definiujemy, używając słowa kluczowego struct (ang. structure -
struktura). Składnia takiej definicji wygląda następująco:

struct nazwa_typu
{
typ_pola_1 nazwa_pola_1;
typ_pola_2 nazwa_pola_2;
typ_pola_3 nazwa_pola_3;
...
typ_pola_n nazwa_pola_n;
};

III. Wskaźniki

Wygodnie jest wyobrażać sobie pamięć operacyjną jako coś w rodzaju wielkiej tablicy
bajtów. W takiej strukturze każdy element ( komórka) powinien dać się jednoznacznie identyfikować poprzez swój indeks. Stąd numer danego bajta w pamięci nazywamy jego adresem. Zmienna przechowująca adres miejsca, pod którym jest jakaś informacja to właśnie wskaźnik. Definicję wskaźnika, który jest adresem pojedynczej komórki pamięci operacyjnej przedstawiono poniżej:

    uint8_t var; //zmienna
    uint8_t *ptr; //wskaźnik - posiada typ, ale nie podaje rozmiaru jaki ma wskaźnik

Wszystkie wskaźniki w systemie są jednego rozmiaru. Ponieważ są typem adresowym, więc tego nie deklarujemy.

Zadeklarowany typ wskaźnika mówi nam o tym, jakiego rodzaju dane nam ten wskaźnik wskazuje. Wiemy jak się obchodzić z informacją spod wskaźnika, wiemy czy jest tam liczba ze znakiem czy bez oraz jakiego rozmiaru kryje się tam informacja, co pokazuje poniższy przykład:

uint8_t *ptr1; //1b
uint16_t *ptr2; //2b
uint32_t *ptr3; //4b

Mając ten sam adres nie wiemy ile bitów brać pod uwagę, dlatego potrzebny jest również typ. Ma to duże znaczenie przy inkrementacji wskaźnika przykładowo takiej jak poniżej:

uint8_t ++ //1b+1b

Jeśli typem jest struktura to ++ podniesie wielkość wskaźnika o całą strukturę.

Dodając pola do struktury automatycznie zwiększa się typ.

Co zrobić ze wskaźnikiem?

uint8_t var; //zmienna
uint8_t *ptr; //wskaźnik
uint8_t ptr;
uint8_t *ptr;
ptr++; //inkrementacja adresu
(*ptr)++ //inkrementacja na wartości
ptr = &var; //poznanie adresu zmiennej, ptr - wcześniej stworzony wskaźnik

Zmienna i wskaźnik powinny być tego samego typu.

void fun(uint8_t arg); //kopia zmiennej (z wyjątkiem tablic)
fun (ptr); // działania odbywają się na tym adresie, a nie kopi
fun(&var); //adres zmiennej
fun(uint8_t *arg);

BMP280_t Czujnik;
BMP280_t *pCzujnik; //wskaźnik typu strukturalnego

Czujnik.t1=27; //zmienna
pCzujnik->t1=27; //wskaźnik

Wskaźniki mogą wskazywać na funkcje. Funkcja ma miejsce w pamięci i można do niej skoczyć podając adres. Mechanizm jest wykorzystywany do tworzenia callbacków.

IV. Callbacki

Wywołanie zwrotne. Odwrotność funkcji. Używa się ich do pisania biblioteki.

Przez wskaźnik na funkcję dajemy możliwość wpisania adresu funkcji zewnętrznej, która ma się wykonać wewnątrz tej biblioteki.

Biblioteka wskoczy do kodu spod wskaźnika, wykona go i wróci sama do siebie. W ten sposób możemy reagować na to co się w bibliotece dzieje i co przewidział jej twórca.

Callbacki daje HAL w miejsca reagowania na przerwania. Przekazujemy tylko kod, który ma reagować na przerwanie.

Rejestracja/Wyrejestrowanie 

Callbacki trzeba zarejestrować. Biblioteka musi wiedzieć dokąd ma skoczyć. Musimy podać jej wskaźnik na nową funkcję i to się nazywa rejestracja.

Wyrejestrowany lub nigdy nie zarejestrowany wskaźnik to wartość NULL.

Dobra biblioteka powinna sprawdzić czy istnieje zanim spróbuje wykonać spod niego kod. Jeśli jest NULL tzn. że nie chcemy używać tego wskaźnika, callbacka i nie ma naszej reakcji w tym miejscu biblioteki.

Callback - Rejestrując go przekazujemy wskaźnik do funkcji. Funkcja musi gdzieś istnieć i być gdzieś napisana i możemy przekazać do niej adres. 

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		SamplesReady = 1;
	}
}

V. Obiektowość w C

Zmienne strukturalne pełnią rolę obiektów. Porównanie do struktury jest całkiem na miejscu, chociaż pojawiło nam się kilka nowych elementów, w tym najbardziej oczywiste zastąpienie słowa kluczowego struct przez class.

BMP280_t Czujnik1;
BMP280_t Czujnik2;

Np. każdy interfejs to kolejny obiekt.

uint8_t ReadTemp (BMP280_t *Czujnik1) //wskaźnik na obiekt

Funkcja wie, pod którym adresem lub na jakim interfejsie widnieje sensor.

Do jednej funkcji możemy wstawić dwie zmienne strukturalne (obiekty) np. dwoma inaczej podłączonymi czujnikami.


