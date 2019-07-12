�wiczenie 6
Konstrukcja systemu plik�w

1. Cel �wiczenia

  Nale�y napisa� w �rodowisku systemu Minix program w j�zyku C (oraz skrypt
  demonstruj�cy wykorzystanie tego programu) realizuj�cy podstawowe funkcje
  systemu plik�w.

2. Funkcje programu 

  System plik�w nale�y zorganizowa� w du�ym pliku o zadanej wielko�ci, kt�ry
  b�dzie "wirtualnym dyskiem". Program powinien tworzy� dysk wirtualny, oraz
  dokonywa� zapis�w i odczyt�w w celu zrealizowania podstawowych operacji na
  dysku, zwi�zanych z zarz�dzaniem katalogiem, alokacj� plik�w oraz
  utrzymywaniem unikalno�ci nazw.

3. Zadanie do zrealizowania

  W pliku na dysku nale�y zorganizowa� system plik�w z jednopoziomowym
  katalogiem.  Elementem katalogu jest opis pliku, zawieraj�cy co najmniej
  nazw�, wielko�� i spos�b rozmieszczenia pliku na wirtualnym dysku. Nale�y
  zaimplementowa� nast�puj�ce operacje, dost�pne dla u�ytkownika programu:

	- tworzenie wirtualnego dysku,
	
	- kopiowanie pliku z dysku systemu Minix na dysk wirtualny,
	
	- kopiowanie pliku z dysku wirtualnego na dysk systemu Minix,
	
	- wy�wietlanie katalogu dysku wirtualnego,
		
	- usuwanie pliku z wirtualnego dysku,
	
	- usuwanie wirtualnego dysku,

	- wy�wietlenie zestawienia z aktualn� map� zaj�to�ci wirtualnego dysku -
	  czyli listy kolejnych obszar�w wirtualnego dysku z opisem: adres, typ
	  obszaru, rozmiar, stan (np. dla blok�w danych: wolny/zaj�ty).

  Program ma kontrolowa� wielko�� dost�pnego miejsca na wirtualnym dysku i
  pojemno�� katalogu, reagowa� na pr�by przekroczenia tych wielko�ci. 

  Nie trzeba realizowa� funkcji otwierania pliku ani czytania/pisania
  fragment�w pliku.

  Nie trzeba realizowa� funkcji zwi�zanych z wsp�bie�nym dost�pem. Zak�ada
  si� dost�p sekwencyjny i wy��czny do wirtualnego dysku.

  Nale�y przygotowa� demonstracj� (zgrupowanie serii polece� w postaci skryptu
  interpretera sh) prezentuj�c� s�abe i silne strony przyj�tego rozwi�zania w
  kontek�cie ewentualnych zewn�trznej i wewn�trznej fragmentacji.


