�wiczenie 5
Zarz�dzanie pami�ci�


1. Cel �wiczenia

Domy�lnie w systemie Minix algorytmem wyboru wolnego bloku z listy
wolnych blok�w, wykorzystywanym do realizacji funkcji systemowych FORK i
EXEC, jest algorytm first fit, czyli wybierany jest pierwszy blok
pami�ci o wystarczaj�cym rozmiarze z listy blok�w wolnych.

Celem �wiczenia jest zmiana domy�lnego algorytmu przydzia�u pami�ci w
systemie Minix. Nale�y umo�liwi� wyb�r algorytmu wyboru bloku z listy
blok�w wolnych mi�dzy standardowym first fit a tzw. algorytmem worst
fit, czyli takim, w kt�rym wybierany jest blok pami�ci z listy wolnych
blok�w o najwi�kszym rozmiarze.

Nale�y zaimplementowa� w systemie algorytm worst fit, a nast�pnie
zademonstrowa� i zinterpretowa� r�nice w dzia�aniu poszczeg�lnych
algorytm�w.


2. Zadanie do zrealizowania

Nale�y zdefiniowa� dwie dodatkowe funkcje systemowe, identyfikowane sta�ymi
HOLE_MAP oraz WORST_FIT.


2.1. Funkcja systemowa HOLE_MAP powinna umo�liwia� zdefiniowanie w�asnej
funkcji o sygnaturze:

int hole_map( void *buffer, size_t nbytes )

kt�ra ma za zadanie zwr�ci� w buforze buffer o rozmiarze nbytes informacje o
aktualnej zawarto�ci listy wolnych blok�w utrzymywanej przez modu�
zarz�dzania pami�ci� (MM). Struktura otrzymanej w buforze informacji powinna
by� nast�puj�ca: 
  
  rozmiar1, adres1, rozmiar2, adres2, ..., 0

gdzie kolejne pary rozmiar, adres odpowiadaj� informacjom o kolejnych
elementach listy wolnych blok�w. Rozmiar 0 oznacza ostatni element listy.
Elementy rozmiar i adres maj� typ danych unsigned int (na poziomie modu�u MM
synonim tego typu o nazwie phys_clicks).

Funkcja hole_map ma zwraca� przes�an� liczb� par rozmiar,adres. Nale�y
zabezpieczy� si� przed przepe�nieniem zadanego jako argument wywo�ania
bufora i wype�ni� go tylko liczb� par mieszcz�cych si� w buforze dbaj�c o
zako�czenie listy pozycj� rozmiar=0.


2.2. Funkcja systemowa WORST_FIT powinna umo�liwia� wyb�r algorytmu wyboru
elementu z listy wolnych blok�w i zdefiniowanie w�asnej funkcji o
sygnaturze:

int worst_fit( int w )

kt�ra dla w=1 wymusza implementowany w ramach �wiczenia algorytm przydzia�u
worst fit, natomiast dla w=0 uaktywnia z powrotem standardowy algorytm first
fit. Warto�ci� zwracan� powinno by� zawsze 0.


3. W celu realizacji zadania nale�y przede wszystkim zapozna� si� z
zawarto�ci� pliku 

/usr/src/mm/alloc.c

i w pliku tym dodatkowo zaimplementowa� odpowiednio funkcje:

PUBLIC int do_worst_fit( void );
PUBLIC int do_hole_map();

argumenty wej�ciowe znajduj� si� w zmiennej globalnej mm_in typu message
przekazanej przez programy u�ytkowe w wywo�aniu _syscall(). Do przekazywania
jako argument�w liczb ca�kowitych mo�na wykorzysta� pole m1_i1 struktury
message, a do przekazania wskazania na bufor pole m1_p1 struktury message.

Do przesy�ania zawarto�ci bufor�w mi�dzy pami�ci� systemu operacyjnego a
pami�ci� program�w u�ytkowych nale�y wykorzysta� funkcj� sys_copy(), kt�rej
przyk�ad u�ycia mo�na znale�� w realizacji funkcji systemowej READ
(w pliku /usr/src/fs/read.c).


4. Testowanie funkcjonalno�ci systemu

Nale�y stworzy� programy u�ytkowe t.c, w.c oraz x.c z nast�puj�c�
zawarto�ci�:

-------------------------------------------------------------
/* t.c - polecenie t wyswietla liczbe i rozmiary blokow wolnych */
#include <stdio.h>
#include <unistd.h>
#include <lib.h>
                                                                                
PUBLIC int hole_map( void *buffer, size_t nbytes)
{
	/* ... _syscall(..HOLE_MAP..) ... */
}
                                                                                
int
main( void )
{
        unsigned int    b[1024];
        unsigned int    *p, a, l;
        int     res;

		res = hole_map( b, sizeof( b ) );
        printf( "[%d]\t", res );
        p = b;
        while( *p )
        {
                l = *p++;
                a = *p++; /* tu niewykorzystywane */
                printf( "%d\t", l );
        }
        printf( "\n" );
        return 0;
}
-------------------------------------------------------------
/* w.c - polecenie w przyjmuje jako argument 1 albo 0 */
/* wlaczajac/wylaczajac algorytm worst fit w systemie Minix */
#include <stdlib.h>
#include <unistd.h>
#include <lib.h>


PUBLIC int worst_fit( int w )
{
	/* ... _syscall(..WORST_FIT..) ... */
}

int
main( int argc, char *argv[] )
{
	if( argc < 2 )
		return 1;
	worst_fit( atoi( argv[1] ) );
	return 0;
}
-------------------------------------------------------------
/* x.c - program pomocniczy x, okrojona wersja polecenia sleep */
/* wykorzystywana do test�w */
#include <stdlib.h>
#include <unistd.h>

int
main( int argc, char *argv[] )
{
	if( argc < 2 )
		return 1;
	sleep( atoi( argv[1] ) );
	return 0;
}
-------------------------------------------------------------

Po przygotowaniu powy�szych polece� nale�y zinterpretowa� rezultat
dzia�ania poni�szego skryptu. Do czego s�u�y polecenie chmem (man
chmem)?


#!/bin/sh
# skrypt do testowania dzia�ania funkcji systemowych 
# HOLE_MAP oraz WORST_FIT
cc -o t t.c 
cc -o w w.c
cc -o x x.c
chmem =8000 x

echo "-[ std ]----------------------------------------"
./w 0
for i in 1 2 3 4 5 6 7 8 9 10
do
	./x 10 &
	./t
	sleep 1
done
for i in 1 2 3 4 5 6 7 8 9 10
do
	./t
	sleep 1
done
echo "-[ worst ]--------------------------------------"
./w 1
for i in 1 2 3 4 5 6 7 8 9 10
do
	./x 10 &
	./t
	sleep 1
done
for i in 1 2 3 4 5 6 7 8 9 10
do
	./t
	sleep 1
done
echo "-[ std ]----------------------------------------"
./w 0


5. Uwagi do realizacji zadania

* lista modyfikowanych plik�w systemowych:

/usr/src/mm/proto.h
/usr/src/mm/alloc.c
/usr/src/mm/table.c
/usr/include/minix/callnr.h

* lista tworzonych program�w u�ytkowych:

w.c
t.c
x.c
skrypt_testowy

* algorytm worst fit wyboru bloku wolnego powinien by� zrealizowany jako
fragment funkcji alloc_mem,

* przy realizacji w�asnego algorytmu wyboru bloku wolnego dopuszczalne
jest zaniedbanie aspekt�w zwi�zanych z wymiataniem (swap),

* nale�y zwr�ci� uwag�, �e rozmiar blok�w przechowywany i wy�wietlany
jest nie w bajtach a w jednostkach click,

* nale�y pami�ta� o regularnym zapisywaniu zmian w plikach �r�d�owych na
osobnej dyskietce,

* w celu poprawy warsztatu pracy w �atwy spos�b mo�na powi�kszy� liczb�
konsoli systemowych z dw�ch do np. czterech poprzez zmian� definicji sta�ej
NR_CONS w pliku /usr/include/minix/config.h


