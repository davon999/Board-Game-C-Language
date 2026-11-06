# Tema-PA 2026

Completati aici documentatia proiectului:
    - descriere funcționlitatea în mare
    - motivație soluțiile alese, complexități

Poturlu David-Andrei 311AB

Structuri de date utilizate:

-Coada (lista simplu inlantuita)
    am implementat coada cu o lista simplu inlantuita, pastrand pointerii
    front si rear ca sa pot insera la final si sa pot extrage de la inceput in 0(1)

Complexitate:
    enqueue (adaugare la final): O(1)
    dequeue (extragere de la inceput): O(1)
    parcurgere: O(n)

-Stiva (lista simplu inlantuita)
    am implementat stiva cu o lista simplu inlantuita, cu un pointer la
    varful stivei - top. Inserarea si extragerea se fac mereu din varf, respectand
    principiul LIFO. Misiunile sunt stocate in stiva, astfel incat ultima misiune citita este prima scoasa.

Complexitate:

    push (adaugare în varf): O(1)
    pop (extragere din varf): O(1)

-Arbore binar de cautare
    jucatorrii sunt inserati in BST folosind energia ca si cheie de comparare. jucatorii cu enrgie mai mica se duc in stanga iar cei cu energie mai mare sau egala in subarborele drept. inserarea o fac recursiv.

Complexitate:
    inserare - O(h) unde h e inaltimea arborelui

    parcurgere postordine - O(n)

-Arbore concurs
    convertesc BST ul intr un arbore de tip concurs ce contine doua tipuri de noduri: noduri jucator si noduri lupta - care conecteaza doi jucatori ce urmeaza sa se lupte
    conversia o realizez recursiv si tratez cele 4 cazuri. fiecare nod primeste un label ce concateneaza id urile jucatorilor implicati in lupta

Complexitate: 
    converie - O(n) n = nr noduri din BST

-Simularea luptelor
    luptele se desfasoara de la frunzele arborelui de concurs catre radacina. la fiecare lupta avanseaza conform cerintei jucatorul cu energia mai mare ( in cazul egalitatii merge mai departe cel din stanga), iar energia castigatirului devine conform regulii : max dintre cei doi + mdulul diferentei energiilor lor
    pierzatorii sunt colectati pe parcurs,iar castigatirul final ramane in radacina. selectez primii k jucatori sortati dupa energie sortati descresc cu qsort (k-cea mai mica putere a lui 2 < n)

Complexitate:
    fight_and_collect - O(n) pt parcurgere si lupte
    qsort - O(nlogn)
    selectare primii k - O(nlogn)

    pierzatorii de la fiecare lupta si castigatorul ii stochez intr un singur vector ca sa sortez cu qsort, am ales asta deoarece este eficientt - se parcurge arborele o singura data O(n) si sortarea finala O(nlogn)

-MinHeap(coada cu prioritati)
    pentru algoritmul astar am implementat un min heap ce mentine prioritatea ca elementul cu valoarea f minima este mereu pe prima pozitie.

Complexitate:
    inserare - O(logn) - heap_push
    extragere - O(log n) - heap_pop
    accesare - O(1)

-Algoritmul A*
    este utilizat pentru gasirea celui mai scurt drum pe harta de la sursa la destinatia fiecarei misiuni.
    functia de cost : f=g+min_cost*h
    unde:
    g-cost real de drum de la sursa la nodul n
    h-distanta Manhattan de la n la destinatie
    min_cost-costul minim din matricele de cost
    la fiecare pas se extrage nodul cu f min din heap,se exploreaza vecinii si se actualizeaza costurile daca se gaseste un drum mai scurt.

Complexitate:
    timp: O(ElogV) unde 
    V-nr de casute accesibilie
    E-nr de muschh (maxim 4*V)
    spatiu: O(V) pt matricele g,visited,prev_x,prev_y si heap

Reprezentarea hartii:
harta este stocata in 3 matrice alocate dinamic:
-matrice val casute:n_lin n_col 0 pt liber, 1 pt obstacol
-matrice cost linii:n_lin si n_col-1 costurile orizontale
-matrice cost coloane:n_lin-1 si n_col pt costuri verticale

Citirea se face cu strtok din fisier linie cu linie alternand intre linii ce contin valori si costurtile oriz/vert

Toata memoria alocata este eliberata ulterior utilizatarii corespunzator cu ajutorul functiilor create

FISIERE SURSA:
1.biblioteca.h - declaratii structuri si functii
2.biblioteca.c - implementarea tuturor functiilor
3.main.c-fluxul principal al programului
