#include "biblioteca.h"

int main(){

    /*PAS 1 - initializare si scriere jucatori*/

    Queue *q=malloc(sizeof(Queue));
    init_queue(q);
    citire_jucatori("Pas_1/jucatori.csv", q);
    scriere_test_1_1("Pas_1/test_1_1.csv", q);

    /*PAS 1.2 - citire harta si separare in 3 matrice*/

    harta *h=malloc(sizeof(harta));
    citire_harta("Pas_1/harta.txt", h);
    scriere_test_1_2("Pas_1/test_1_2.txt", h);

    /*PAS 1.3 - citire misiuni si stocare in stiva*/

    stack *s=malloc(sizeof(stack));
    init_stack(s);
    citire_misiuni("Pas_1/misiuni.csv", s);
    scriere_test_1_3("Pas_1/test_1_3.csv", s);

    /*PAS 2.1 - se construieste BST ul cu jucatorii din coada*/

    BSTNode *bst=NULL;
    int n=0;
    QueueNode *curr=q->front;
    while(curr){
        bst=insert_BST(bst, curr->data);
        n++;
        curr=curr->next;
    }

    /*convertia din BST in arbore de concurs*/

    BSTNode *contest=convert_to_contest(bst);

    /*afisare arbore in postordine*/
    FILE *f=fopen("Pas_2/test_2_1.txt","w");
    postorder(contest, f);
    fclose(f);

    /*PAS 2.2 - simulare lupte si selectarea primilor k jucatori*/

    int k = find_k(n);
    jucator *all = malloc(n * sizeof(jucator));
    int count = 0;

    /*colectarea pierzatorilor din lupte si a castigatorului final*/
    fight_and_collect(contest, all, &count);
    all[count] = contest->data; // castigatorul final
    count++;

    /*sortare descrescatoare dupa energie*/
    qsort(all, count, sizeof(jucator), cmp_desc);

    /*scrierea primilor k jucatori*/
    FILE *f2 = fopen("Pas_2/test_2_2.txt", "w");
    for (int i = 0; i < k; i++) {
    fprintf(f2, "J%d %.2f\n", all[i].id, all[i].energie);
    }
    fclose(f2);

   /*PAS 3 - stribui misiunlor si se rezolva cu algoritmul astar*/
    scriere_test_3("Pas_3/test_3.txt", all, k, s, h);

    /*eliberez memoria*/
    free(all);
    free_queue(q);
    free(q);
    free_harta(h);
    free(h);
    free_stack(s);
    free(s);
    free_tree(contest);
    
    return 0;
}