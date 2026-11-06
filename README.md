# Temă Proiectarea Algoritmilor 2025-2026 — Board-game

## Descriere generală

Proiectul simulează un board-game care include o hartă, jucători și misiuni.
Jucătorii sunt citiți dintr-un fișier CSV, organizați într-o coadă, apoi introduși
într-un arbore binar de căutare (BST) pentru a simula lupte. Câștigătorii primesc
misiuni pe care le rezolvă folosind algoritmul A* pe harta jocului.

## Structuri de date utilizate

### Coadă (listă simplu înlănțuită)

Am implementat coada cu o listă simplu înlănțuită, păstrând pointeri la primul
(front) și ultimul (rear) element. Această alegere permite inserarea la final
și extragerea de la început în O(1).

Complexitate:
- enqueue (adăugare la final): O(1)
- dequeue (extragere de la început): O(1)
- parcurgere: O(n)

### Stivă (listă simplu înlănțuită)

Stiva a fost implementată tot cu o listă simplu înlănțuită, cu un pointer la
vârful stivei (top). Inserarea și extragerea se fac mereu din vârf, respectând
principiul LIFO (Last In, First Out). Misiunile sunt stocate în stivă, astfel
încât ultima misiune citită este prima scoasă.

Complexitate:
- push (adăugare în vârf): O(1)
- pop (extragere din vârf): O(1)

### Arbore binar de căutare (BST)

Jucătorii sunt inserați într-un BST folosind energia ca și cheie de comparare.
Jucătorii cu energie mai mică se duc în subarborele stâng, cei cu energie mai
mare sau egală în subarborele drept. Inserarea se face recursiv.

Complexitate:
- inserare: O(h), unde h este înălțimea arborelui; în cazul mediu O(log n),
  în cazul cel mai defavorabil (arbore degenerat) O(n)
- parcurgere postordine: O(n)

### Arbore concurs

BST-ul este convertit într-un arbore concurs printr-o transformare bottom-up
(de la frunze la rădăcină). Arborele concurs conține două tipuri de noduri:
noduri jucător (care păstrează datele unui jucător) și noduri luptă (care
conectează doi jucători ce urmează să se lupte).

Conversia se face recursiv, tratând 4 cazuri în funcție de structura
subarborelui curent (subarbore plin, doar copil stâng, doar copil drept,
sau copii deja convertiți). Fiecare nod primește un label care concatenează
id-urile jucătorilor implicați.

Complexitate conversie: O(n), unde n este numărul de noduri din BST.

### Simularea luptelor

Luptele se desfășoară de la frunzele arborelui concurs către rădăcină. La
fiecare nod de tip luptă, câștigă jucătorul cu energia mai mare (în caz de
egalitate, câștigă cel din stânga). Energia câștigătorului devine:

    Ejc = max(E1, E2) + |E1 - E2|

Pierzătorii sunt colectați pe parcurs, iar câștigătorul final rămâne în
rădăcină. Se selectează primii k = 2^p jucători (cea mai mare putere a lui 2
mai mică sau egală cu n), sortați descrescător după energie folosind qsort.

Complexitate:
- fight_and_collect (parcurgere + lupte): O(n)
- qsort pe array-ul de jucători: O(n log n)
- total selectare primii k: O(n log n)

Am ales să colectez pierzătorii la fiecare luptă și câștigătorul final
într-un singur array, apoi să sortez cu qsort. Aceasta este o soluție
eficientă deoarece parcurge arborele o singură dată (O(n)) și sortarea
finală este O(n log n).

### Min-Heap (coadă cu priorități)

Pentru algoritmul A* am implementat un min-heap bazat pe array dinamic.
Heap-ul menține proprietatea că elementul cu valoarea f minimă este mereu pe
prima poziție. Suportă realocare dinamică atunci când capacitatea este
depășită.

Complexitate:
- inserare (heap_push): O(log n) — elementul urcă în heap
- extragere minim (heap_pop): O(log n) — elementul coboară în heap
- accesare minim: O(1)

### Algoritmul A*

Algoritmul A* este utilizat pentru a găsi drumul cel mai scurt pe hartă de la
sursa la destinația fiecărei misiuni. A* extinde algoritmul Dijkstra prin
adăugarea unei euristici care estimează distanța rămasă până la destinație.

Funcția de cost: f(n) = g(n) + min_cost * h(n), unde:
- g(n) = costul real al drumului de la sursă la nodul n
- h(n) = distanța Manhattan de la n la destinație
- min_cost = costul minim din matricele de cost (pentru admisibilitate)

Ponderarea euristicii cu min_cost garantează că estimarea nu supraestimează
costul real, asigurând optimalitatea soluției.

La fiecare pas, se extrage nodul cu f minim din heap, se explorează vecinii
(sus, jos, stânga, dreapta), și se actualizează costurile dacă se găsește
un drum mai scurt. Dacă energia jucătorului nu este suficientă pentru a
parcurge un drum (energie - g <= 0), acel drum este exclus.

Calea finală se reconstruiește urmărind matricele prev_x și prev_y de la
destinație la sursă.

Complexitate A*:
- timp: O(E log V), unde V = numărul de căsuțe accesibile, E = numărul de
  muchii (maxim 4 * V). Cu heap-ul, fiecare operație de inserare/extragere
  este O(log V), iar fiecare nod este procesat cel mult o dată.
- spațiu: O(V) pentru matricele g, visited, prev_x, prev_y și heap.

## Reprezentarea hărții

Harta este stocată în 3 matrici alocate dinamic:
- matrice valori căsuțe (int **): n_lin x n_col — 0 = liber, 1 = obstacol
- matrice cost linii (float **): n_lin x (n_col-1) — costuri orizontale
- matrice cost coloane (float **): (n_lin-1) x n_col — costuri verticale

Citirea se face parsând fișierul linie cu linie cu strtok, alternând între
linii care conțin valori + costuri orizontale și linii cu costuri verticale.

## Managementul memoriei

Toată memoria alocată dinamic este eliberată corespunzător:
- free_queue: eliberează nodurile cozii și string-urile prenume_nume
- free_stack: eliberează nodurile stivei
- free_harta: eliberează cele 3 matrici ale hărții
- free_tree: eliberează recursiv nodurile arborelui și label-urile
- în astar: se eliberează matricele g, visited, prev_x, prev_y și heap-ul

Programul a fost verificat cu Valgrind fără erori de memorie.

## Fișiere sursă

- biblioteca.h: declarații de structuri, enum-uri și prototipuri de funcții
- biblioteca.c: implementarea tuturor funcțiilor
- main.c: fluxul principal al programului
