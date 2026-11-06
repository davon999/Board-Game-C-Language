
Board Game în C

O simulare de board-game scrisă în C: jucătorii se înscriu, se luptă într-un turneu eliminatoriu construit pe arbori binari, iar câștigătorii primesc misiuni pe o hartă cu obstacole, unde drumul cel mai scurt este găsit cu algoritmul A*.

Proiectul este construit în jurul structurilor de date clasice, implementate de la zero: coadă, stivă, arbore binar de căutare, arbore de turneu și min-heap.
Cum funcționează

Simularea se desfășoară în trei etape.
1. Înscrierea jucătorilor, harta și misiunile

    Jucătorii sunt citiți dintr-un fișier .csv și puși într-o coadă, în ordinea citirii. Fiecare primește un id unic, începând de la 0.
    Harta este citită dintr-un fișier text și separată în trei matrice: valorile căsuțelor (liber/obstacol), costurile de trecere pe orizontală și costurile de trecere pe verticală.
    Misiunile (punct de start → punct final) sunt citite dintr-un .csv și puse într-o stivă.

Un jucător are:
Câmp	Tip	Observații
prenume_nume	char *	alocat dinamic, cât e nevoie
energie	float	
abilitate	enum	NEUTRALITATE, REINCERCARE, RENASTERE
id	int	asignat în ordinea citirii
2. Turneul

    Jucătorii sunt scoși din coadă și inserați într-un BST, ordonat după energie.
    BST-ul este convertit într-un arbore de turneu: frunzele sunt jucători, iar nodurile interne sunt lupte (><) între jucătorii de sub ele.
    Luptele se dau de la frunze spre rădăcină. Câștigă jucătorul cu energia mai mare (la egalitate, cel din stânga), iar energia lui devine:

   E_castigator = max(E1, E2) + |E1 - E2|

    La final se aleg primii k jucători după energia rămasă, unde k este cea mai mare putere a lui 2 pentru care k ≤ n (n = numărul de jucători).

Exemplu pentru trei jucători cu energiile J0 = 15, J1 = 5, J2 = 17:

       BST                        Arbore de turneu

     J0 (15)                          J1J0J2
     /     \                          /     \
 J1 (5)   J2 (17)                  J1J0      J2
                                  /    \
                                J1      J0

    J1 (5) vs J0 (15) → câștigă J0, cu 15 + 10 = 25
    J0 (25) vs J2 (17) → câștigă J0, cu 25 + 8 = 33

Clasament (k = 2): J0 33.00, J2 17.00
3. Misiunile pe hartă

Fiecare câștigător, în ordinea descrescătoare a energiei, primește următoarea misiune scoasă din stivă. Drumul de la start la destinație este căutat cu A*. Fiecare pas consumă din energia jucătorului costul trecerii dintre căsuțe; dacă energia ajunge la 0 sau sub, misiunea eșuează.

Exemplu de rezultat:

J0 - 21.00: [0, 0] -- [1, 0] -- [1, 1] -- [0, 1] -- [0, 2]
J2: Nu s-a putut efectua misiunea

Structuri de date și algoritmi
Coadă (listă simplu înlănțuită)

Păstrez pointerii front și rear, ca inserarea la final și extragerea de la început să se facă în timp constant.
Operație	Complexitate
enqueue (adăugare la final)	O(1)
dequeue (extragere de la început)	O(1)
parcurgere	O(n)
Stivă (listă simplu înlănțuită)

Un singur pointer, top, către vârful stivei. Inserarea și extragerea se fac mereu din vârf (LIFO), deci ultima misiune citită este prima scoasă.
Operație	Complexitate
push (adăugare în vârf)	O(1)
pop (extragere din vârf)	O(1)
Arbore binar de căutare

Jucătorii sunt inserați recursiv, cu energia drept cheie: energie mai mică → subarborele stâng, energie mai mare sau egală → subarborele drept.
Operație	Complexitate
inserare	O(h), h = înălțimea arborelui (O(log n) în medie, O(n) dacă arborele degenerează)
parcurgere postordine	O(n)
Arborele de turneu

Conversia BST → arbore de turneu se face recursiv, de la frunze spre rădăcină. Pentru fiecare subarbore tratez patru cazuri:

    subarbore plin: frunza stângă și părintele se leagă printr-un nod de luptă, care se leagă apoi printr-un alt nod de luptă cu frunza dreaptă;
    doar fiu stâng: frunza și părintele se leagă printr-un nod de luptă;
    doar fiu drept: părintele și frunza se leagă printr-un nod de luptă;
    fii care sunt deja noduri de luptă: se aplică aceleași reguli, nodurile de luptă fiind tratate ca noduri jucător.

Fiecare nod primește o etichetă formată din id-urile jucătorilor implicați (ex. J1J0J2), care poate fi afișată în postordine.
Operație	Complexitate
conversie	O(n), n = numărul de noduri din BST
Simularea luptelor și selecția primilor k

fight_and_collect parcurge arborele o singură dată: la fiecare nod de luptă decide câștigătorul, îi actualizează energia și pune învinsul într-un vector. Câștigătorul final rămâne în rădăcină și este adăugat și el în vector. Vectorul este apoi sortat descrescător după energie cu qsort, iar primii k jucători sunt cei de la începutul lui.
Pas	Complexitate
parcurgere și lupte	O(n)
sortare cu qsort	O(n log n)
selecția primilor k	O(k)
total	O(n log n) timp, O(n) spațiu

De ce așa: arborele este parcurs o singură dată, iar fiecare jucător ajunge în vector exact o dată, cu energia de după ultima lui luptă. Cum k este cea mai mare putere a lui 2 cu k ≤ n, avem mereu k > n/2, deci trebuie selectată oricum cel puțin jumătate din jucători. O selecție parțială (de exemplu cu un heap de dimensiune k) ar costa O(n log k) = O(n log n), adică la fel, așa că sortarea completă e cea mai simplă soluție, fără pierdere de eficiență.
Min-heap (coadă cu priorități)

Folosit de A*. Păstrează mereu în vârf nodul cu cea mai mică valoare f.
Operație	Complexitate
inserare (heap_push)	O(log n)
extragere minim (heap_pop)	O(log n)
acces la minim	O(1)
Algoritmul A*

Găsește cel mai scurt drum pe hartă de la sursa la destinația fiecărei misiuni. Funcția de cost:

f(n) = g(n) + min_cost * h(n)
h(n) = |n.x - d.x| + |n.y - d.y|      (distanța Manhattan)

    g(n): costul real al drumului de la sursă până la n
    h(n): distanța Manhattan de la n la destinația d
    min_cost: cel mai mic cost din matricele de cost

La fiecare pas extrag din heap nodul cu f minim, explorez cei (cel mult) 4 vecini accesibili și le actualizez costul dacă am găsit un drum mai scurt. Un vecin este adăugat în heap doar dacă jucătorului îi rămâne energie (energie - g > 0). Drumul este reconstruit la final din matricele prev_x / prev_y.

De ce min_cost * h(n): orice pas pe hartă costă cel puțin min_cost, deci min_cost * h(n) nu depășește niciodată costul real rămas până la destinație. O euristică care nu supraestimează garantează că A* găsește drumul optim.
	Complexitate
timp	O(E log V), unde V = numărul de căsuțe accesibile și E = numărul de muchii (E ≤ 4V), deci O(V log V)
spațiu	O(V), pentru matricele g, visited, prev_x, prev_y și heap
Reprezentarea hărții

Harta este stocată în trei matrice alocate dinamic:
Matrice	Dimensiune	Conținut
valori căsuțe	n_lin × n_col	0 = liber, 1 = obstacol
cost linii	n_lin × (n_col - 1)	costurile de trecere pe orizontală
cost coloane	(n_lin - 1) × n_col	costurile de trecere pe verticală

Fișierul de intrare alternează două tipuri de linii: linii cu valorile căsuțelor intercalate cu costurile orizontale, și linii cu costurile verticale. Citirea se face linie cu linie, cu strtok.

0  12  0  1  0
6      3     5
0  2   0  1  1
12     7     4
0  4   1  1  0

Pentru exemplul de mai sus, harta are 3 × 3 căsuțe: linia 0 12 0 1 0 înseamnă căsuța (0,0) = 0, cost 12 până la (0,1), căsuța (0,1) = 0, cost 1 până la (0,2), căsuța (0,2) = 0.
Formatul datelor de intrare

Jucători (.csv):

PRENUME NUME;ENERGIE;ABILITATE
Mirel Alexandru;15;NEUTRALITATE
Gabi Manea;5;RENASTERE
Viorel-Bogdan Protop;17;REINCERCARE

Misiuni (.csv), coordonate în matricea hărții:

Xi;Yi;Xf;Yf
2;0;0;0
0;0;0;2

Structura proiectului

.
├── src/
│   ├── biblioteca.h   # declarațiile structurilor și ale funcțiilor
│   ├── biblioteca.c   # implementarea tuturor funcțiilor
│   └── main.c         # fluxul principal al programului
├── Makefile
└── README.md

Compilare:

make

Gestionarea memoriei

Toată memoria alocată dinamic (jucători, nume, noduri de coadă/stivă/arbore, matricele hărții, heap-ul) este eliberată după utilizare, prin funcții dedicate fiecărei structuri.

valgrind --leak-check=full ./<executabil>

Autor

Poturlu David-Andrei