
�wiczenie 4
Synchronizacja proces�w z wykorzystaniem monitor�w


1. Cel �wiczenia

Nale�y napisa� program w j�zyku C++ w �rodowisku systemu Linux realizuj�cy
rozwi�zanie problemu zadanego przez prowadz�cego �wiczenie. Problem jest
rozwini�ciem �wiczenia nr 3.


2. Operacje monitorowe 

Przyk�adowa notacja monitora (za Hoare) jest nast�puj�ca:

type mon = monitor                                 //1
   var ...{zmienne wewn�trzne monitora};
   var c1,...,cP:condition;                        //2
procedure entry X (...);
var ...
begin                                              //3
   if ... then ci.wait;                            //4
   ... {w�a�ciwa tre�� procedury}
   if not cj.empty                                 //5 
   then cj.signal                                  //6
end;                                               //7
procedure entry Y ...
...

begin 
  {inicjowanie zmiennych wewn�trznych}             //8
end.

Do realizacji monitora s�u�y specjalna biblioteka realizuj�ca nast�puj�ce
elementy monitora, odwo�uj�ce si� do wskazanych miejsc w kodzie: 

// 1 typ "mon" s�u��cy do "powo�ywania" monitor�w, w szczeg�lno�ci
// zawieraj�cych zmienn� semaforow� do wzajemnego wykluczania "mutex". Ka�dy
// monitor musi zawiera� dok�adnie jedn� zmienn� typu "mon": m:mon.

// 2 typ warunk�w Hoare'a, pozwalaj�cy na powo�ywanie warunk�w
// synchronizuj�cych. 

// 3 wej�cie do monitora "enter". Musi by� umieszczone na pocz�tku ka�dej
// metody monitora i tylko tam: enter(m);

// 4 zawieszenie na warunku: wait(ci);

// 5 badanie niepusto�ci warunku: empty(cj);

// 6 wznawianie procesu: signal(cj);

// 7 wyj�cie z monitora "leave". Musi by� umieszczone na ko�cu ka�dej metody
// monitora i tylko tam: leave(m);

// 8 inicjowanie monitora i zmiennych warunkowych: initm(m); initc(c1,m); ...


3. Zadanie do zrealizowania

Nale�y zrealizowa� typ "bufor komunikacyjny" przy pomocy monitora. W czasie
implementacji nale�y zapewni� synchronizacj� tak� sam� jak w �wiczeniu 3.

Nale�y zrealizowa� wi�zy pomi�dzy buforami, te same co w �wiczeniu 3,
zmodyfikowane o dodatkowe warunki wskazane dla �wiczenia 4 przez prowadz�cego.

