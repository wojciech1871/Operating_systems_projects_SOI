
�wiczenie 3
Synchronizacja proces�w z wykorzystaniem semafor�w

1. Cel �wiczenia

Nale�y napisa� program w j�zyku C w �rodowisku systemu Linux realizuj�cy
rozwi�zanie problemu zadanego przez prowadz�cego �wiczenie. Problem jest
zwi�zany z organizacj� N bufor�w komunikacyjnych, pomi�dzy kt�rymi s�
narzucone dodatkowe wi�zy.

2. Bufor komunikacyjny

Bufor komunikacyjny jest struktur� danych mieszcz�c� maksymalnie M element�w
jednakowego typu. Elementy z bufora mog� by� wyjmowane w kolejno�ci
umieszczania (FIFO) lub odwrotnej (LIFO, stos). 

3. Zadanie do zrealizowania

Nale�y zrealizowa� typ "bufor komunikacyjny". W czasie implementacji nale�y
zapewni� synchronizacj�:

- nie dopu�ci� do czytania z pustego bufora,

- nie dopu�ci� do zapisu do pe�nego bufora,

- zadba� o "nie przeszkadzanie sobie" proces�w zapisuj�cych do bufora i
  proces�w czytaj�cych z bufora.
  
Dodatkowe wi�zy mog� dotyczy� liczby element�w aktualnie przechowywanych w
buforach, kolejno�ci zapisu do i odczytu z poszczeg�lnych bufor�w, liczby
element�w zapisywanych/odczytywanych jednocze�nie, operacji na wielu buforach
jednocze�nie, itd.

4. Przygotowanie do zaj��

- prosz� zapozna� si� z definicj� semafora oraz przyk�adem zastosowania w
  problemie producent-konsument ze slajd�w wyk�adu. W szczeg�lno�ci s� Pa�stwo
  zobligowani zna� podan� na slajdach dok�adn� definicj� semafora. Prosz� nie
  pomyli� definicji z realizacj�.
  
  http://www.ia.pw.edu.pl/~tkruk/edu/soi.b/wyklad/w04.pdf
  (Semafory: definicja, Semafory: producent-konsument),

- http://pl.wikipedia.org/wiki/Semafor_(informatyka),

- $ man semop/semget/ipcs/ipcrm,

- "5.2 Processes Semaphores" w rozdziale "Advanced Linux Programming'
  dost�pnym on-line:
  http://www.advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf

- wskazanym by�oby te� wprowadzaj�co cho�by przejrzenie rozdzia�u dotycz�cego 
  proces�w:

  http://www.advancedlinuxprogramming.com/alp-folder/alp-ch03-processes.pdf

  $ man 2 fork
  $ info libc "Process Creation Example"


