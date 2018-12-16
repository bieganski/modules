uwagi do implementacji:
- getline alokuje pamiec dynamicznie, dlatego potem trzeba ją zwolnić, uzywamy jej żeby
 wczytana nazwa katalogu mogla byc dowolnie długa.

do kazdego scheme podpieta jest lista modulów, uzylem listy bo moze byc dowolnie dużo modułów.

funkcje init_lists - przyjmuja wskaznik z dwoma gwiazdkami, czyil 'wskaznik na wskaznik',
ktory dziala mniej wiecej tak, ze nie operujemy na kopii przekazanej do funkcji, a tak jakby na samym wskazniku
(dsotajemy adres wskaznika, to jakby poziom wyzej, zmieniamy bezposrednio to co pod tym adresem).



decyzje projektowe:
nie znamy długości listy modułów podpiętych dio schematów, więc powinny być listami
(tablice trzebaby realokować, gdyby sie okazalo ze wzielismy za małą).

podobnie z schemami, nie wiemy ile ich jest, mozna probowac miec tablice realokowaną, ale kod listy na schemach
jest w zasadzie copy-pastem kodu na modulach, wiec tez zdecydowalem sie na liste.

to byly w zasadzie jedyne powazne decyzje projektowe, reszta z nich w miare wynikala.