#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum abilitate{
        NEUTRALITATE,
        REINCERCARE,
        RENASTERE
    };

typedef struct jucator{
    char *prenume_nume;
    float energie;
    int id; /*se incepe cu 0*/
    enum abilitate tip_abilitate;
    
}  jucator;

typedef struct QueueNode{
    jucator data;
    struct QueueNode *next;
}   QueueNode;

typedef struct Queue{
    struct QueueNode *front;
    struct QueueNode *rear;
}   Queue;

typedef struct harta{
    int **valori; /*valori casute : 0 pt liber 1 pt obstacol*/
    float **cost_linii; /* cost depl orizontala */
    float **cost_coloane; /* cost depl verticala */
    int linii;
    int coloane;
}   harta;

typedef struct misiune{
    int xi,yi,xf,yf; /*pozitiie de start si final*/
}   misiune;

/* nod din stiva de misiuni */
typedef struct stackNode{
    misiune data;
    struct stackNode *next;
}   stackNode;

typedef struct stack{
    struct stackNode *top;
}   stack;

typedef struct BSTNode{
    jucator data;
    int is_fight; // verificarea daca este jucator sau lupta; 0 pt jucator si 1 pt lupta;
    char *label;
    struct BSTNode *left;
    struct BSTNode *right;
}   BSTNode;

typedef struct HeapNode{
    int x,y; /*coordonate casute */
    float f,g; /*f=g+h costul total estimat iar g = costul real de la sursa pana aici*/
}   HeapNode;

typedef struct MinHeap{
    HeapNode *data;
    int size;
    int capacity;
}   MinHeap;

void init_queue(Queue *q);
void free_queue(Queue *q);
void enqueue(Queue *q,jucator j);

void citire_jucatori(const char *filename, Queue *q);
void scriere_test_1_1(const char *filename, Queue *q);

void citire_harta(const char *filename, harta *h);
void scriere_test_1_2(const char *filename, harta *h);
void free_harta(harta *h);

void init_stack(stack *s);
void push(stack *s,misiune m);
void citire_misiuni(const char *filename, stack *s);
void scriere_test_1_3(const char *filename, stack *s);
void free_stack(stack *s);

BSTNode *createNode(jucator j);
BSTNode *insert_BST(BSTNode *root, jucator j);
BSTNode *create_fight_node(BSTNode *left, BSTNode *right);
BSTNode *convert_to_contest(BSTNode *root);
void postorder(BSTNode *root, FILE *f);
void free_tree(BSTNode *root);

int find_k(int n);
int cmp_desc(const void *a, const void *b);
void fight_and_collect(BSTNode *root,jucator *results,int *count);
void scriere_test_2_2(const char *filename,BSTNode *contest,int n);

MinHeap *create_heap(int capacity);
void swap_heap(HeapNode *a, HeapNode *b);
void heap_push(MinHeap *heap, HeapNode node);
HeapNode heap_pop(MinHeap *heap);
void free_heap(MinHeap *heap);
float find_min_cost(harta *h);
float get_cost(harta *h, int x1, int y1, int x2, int y2);
void astar(harta *h, jucator *j, misiune *m, FILE *f);
void scriere_test_3(const char *filename, jucator *winners, int k, stack *s, harta *h);

#endif