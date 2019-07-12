
�wiczenie 2. 
Szeregowanie proces�w


1. Cel �wiczenia

Celem  �wiczenia  jest zaprojektowanie mechanizmu szeregowania w systemie
MINIX. W trakcie �wiczenia nale�y zamieni� standardow� procedur� szereguj�c�
zgodnie z algorytmem szeregowania o cechach wskazanych przez prowadz�cego.


2. Szeregowanie w systemie MINIX

W systemie MINIX procesy s� podzielone na trzy klasy: 

- zadania systemowe (TASK)

- procesy serwerowe (SERVER)

- procesy u�ytkowe (USER)

Procesy TASK maja bezwzgl�dny priorytet nad procesami SERVER, a te z kolei nad
procesami USER. Procesy systemowe TASK i SERVER s� realizowane w re�imie
pob�a�aj�cym, to znaczy proces realizuje si� tak d�ugo, a� sam zrzecze si�
procesora.  Proces USER po wyczerpaniu kwantu czasu jest przenoszony na koniec
kolejki proces�w gotowych. Procedura szereguj�ca wybiera do realizacji zawsze
proces b�d�cy g�ow� kolejki proces�w gotowych.


3. Przyk�adowy algorytm szeregowania

Poni�ej przedstawiono przyk�adowy algorytm szeregowania. Prosz� pami�ta�, �e
jets to tylko przyk�ad, a w�a�ciwe zadanie do zrealizowania w ramach
laboratorium znajduje si� w punkcie 6.

W przyk�adowym algorytmie szeregowania obowi�zuj� nast�puj�ce zasady:

- proces ma dwa atrybuty: priorytet bazowy BASE_PRI i priorytet aktualny
  ACT_PRI.

- dwie zmienne systemowe MAX_AGE i MIN_PRI, MAX_AGE>MIN_PRI, dziel�
  szeregowane procesy na trzy kategorie:

	  * priorytet bazowy > MAX_AGE - proces realizowany w re�imie 
	pob�a�ania (oczywi�cie z wyj�tkiem proces�w klas TASK i SERVER), priorytet
	bie��cy zawsze r�wny bazowemu,
    
	  * priorytet bazowy > MIN_PRI, <= MAX_AGE - proces 
	realizowany w re�imie starzenia: proces kt�remu jest zabierany procesor
	otrzymuje priorytet bie��cy r�wny bazowemu, wszystkie inne z tej grupy
	zwi�kszaj� priorytet bie��cy o 1 (maksymalnie do MAX_AGE); proces kt�remu
	jest zabierany procesor jest wstawiany do kolejki za innymi procesami o
	tym samym priorytecie bie��cym,
    
	  * priorytet bazowy < MIN_PRI - proces realizowany w re�imie 
	priorytet�w statycznych z podzia�em czasu: proces kt�remu jest zabierany
	procesor jest wstawiany do kolejki za innymi procesami o tym samym
	priorytecie bie��cym; priorytet bie��cy zawsze r�wny bazowemu.


4. Cechy funkcjonalne przyk�adowego algorytmu w systemie MINIX:

- realizacja szeregowania zgodnie z przedstawionym algorytmem,

- warto�ci pocz�tkowe zmiennych: MAX_AGE=1000, MIN_PRI=100,

- ka�dy nowo tworzony proces otrzymuje priorytet bazowy i bie��cy r�wne
  MIN_PRI,

- system udost�pnia nowe wywo�anie - ustaw parametr szeregowania:

	void set_PRI (int nr; unsigned VALUE) 

	nr = 1  nadaj bie��cemu procesowi priorytet VALUE 
	nr = 2  ustaw zmienna MAX_AGE na VALUE 
	nr = 3  ustaw zmienna MIN_PRI na VALUE

5. Wskaz�wki implementacyjne przy implementacji algorytmu przyk�adowego

a) plik kernel/proc.h:
	
	- w strukturze 'proc' umie�ci� priorytet bazowy i bie��cy procesu,
	
	- zadeklarowa� zmienne systemowe MAX_AGE i MIN_PRI.

b) plik kernel/proc.c:
	
	- w funkcji 'sched()', kt�ra jest procedur� szereguj�c�, zamiast
	  przenoszenia procesu bie��cego na koniec kolejki proces�w gotowych
	  zrealizowa� zasady algorytmu przyk�adowego,
	
	- w funkcji 'ready(rp)', kt�ra budzi proces, zadba� o umieszczenie go w
	  odpowiednim miejscu struktur algorytmu (je�eli jest to proces klasy
	  USER).

c) plik kernel/system.c:
	
	- w funkcji 'do_fork(m_ptr)', kt�ra tworzy nowy proces, w cz�ci
	  dotycz�cej proces�w klasy USER zadba� o odpowiednie zainicjowanie
	  priorytetu bazowego i bie��cego procesu.

d) plik kernel/main.c:
	
	- w funkcji 'main()', na pocz�tku zadba� o w�a�ciwe zainicjowanie
	  zmiennych systemowych MAX_AGE i MIN_PRI, w p�tli powo�uj�cej deskryptory
	  proces�w (for (rp=BEG...)) zadba� o w�a�ciwe wype�nienie priorytet�w, w
	  cz�ci ko�cowej powo�uj�cej proces INIT zadba� r�wnie� o odpowiednie
	  warto�ci priorytet�w.

e) doda� do systemu wywo�anie systemowe zgodnie ze sk�adni� podan� w punkcie
4, z tym, �e odwo�anie ma nast�pi� do samego mikroj�dra za po�rednictwem
jednego z modu��w MM/FS. Dlatego te� funkcja realizuj�ca dodane wywo�anie
systemowe (np. o nazwie do_setpri()) w jednym z modu��w MM/FS powinna mie�
poni�sza posta�:

PUBLIC void do_setpri() 
{ 	
	message m; 
	m = mm_in; 
	_taskcall(SYSTASK, SYS_SETPRI, &m); 
}

gdzie SYS_SETPRI jest zdefiniowana przez nas sta�a w pliku
/usr/include/minix/com.h. 

Ponadto, do pliku /usr/src/kernel/system.c nale�y doda� obs�ug� funkcji o
numerze SYS_SETPRI i zdefiniowa� sam� funkcj� obs�uguj�c� to wywo�anie. W
obs�udze mo�na wykorzysta� makro proc_addr( id ) zwracaj�ce wskazanie na
struktur� proc zadanego procesu.


6. Zadanie do zrealizowania

Prowadz�cy poleci studentom zrealizowa� algorytm szeregowania o zadanych
w�asno�ciach funkcjonalnych. Przyk�ad: zrealizowa� algorytm szeregowania kt�ry
zapewni w�a�ciwe traktowanie proces�w trzech kategorii: interaktywnych (np.
gra w statki), obliczeniowych (obliczenia numeryczne) oraz masywnego
wej�cia/wyj�cia realizowanego w tle (np. proces sk�adowania dysku na ta�mie).

