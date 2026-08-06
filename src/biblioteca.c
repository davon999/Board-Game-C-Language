#include "biblioteca.h"

void init_queue(Queue *q){
    q->front=NULL;
    q->rear=NULL;
}

void enqueue(Queue *q,  jucator j){
    QueueNode *nou=malloc(sizeof(struct QueueNode));
    nou->data=j;
    nou->next=NULL;
    if(q->rear == NULL){
        q->front=nou;
        q->rear=nou;
    }
    else{
        q->rear->next=nou;
        q->rear=nou;
    }
}

void free_queue(Queue *q){
    QueueNode *curr=q->front;
    while(curr){
        QueueNode *temp=curr;
        curr=curr->next;
        free(temp->data.prenume_nume);
        free(temp);
    }
}

void citire_jucatori(const char *filename, Queue *q){
    FILE *f = fopen(filename,"r");
    char linie[256];
    int id  = 0;
    fgets(linie, sizeof(linie), f); /*se sare peste header*/
    while(fgets(linie, sizeof(linie),f)){
        linie[strcspn(linie,"\n")]='\0';

        char nume[128];
        float energie;
        char abilitate_str[32];

        char *p=strtok(linie,";");
        strcpy(nume,p);

        p=strtok(NULL,";");
        energie=atof(p);

        p=strtok(NULL,";");
        strcpy(abilitate_str,p);

        jucator j;
        j.prenume_nume=malloc(strlen(nume)+1);
        strcpy(j.prenume_nume,nume);
        j.energie=energie;
        j.id=id++;

        if(strcmp(abilitate_str,"NEUTRALITATE")==0)
            j.tip_abilitate=NEUTRALITATE;
        else if(strcmp(abilitate_str,"REINCERCARE")==0)
             j.tip_abilitate=REINCERCARE;
        else
            j.tip_abilitate=RENASTERE;

            enqueue(q,j);
    }
    fclose(f);
}

void scriere_test_1_1(const char *filename, Queue *q){
    FILE *f=fopen(filename,"w");
    fprintf(f,"PRENUME NUME;ENERGIE;ABILITATE;ID\n");

    const char *abilitate_str[]={"NEUTRALITATE","REINCERCARE","RENASTERE"};
    QueueNode *curr=q->front;
    while(curr){
        fprintf(f,"%s;%.2f;%s;%d",curr->data.prenume_nume,curr->data.energie,abilitate_str[curr->data.tip_abilitate],curr->data.id);
        if(curr->next){
            fprintf(f,"\n");
        }
        curr=curr->next;
    }
fclose(f);
}

void citire_harta(const char *filename, harta *h){
    FILE *f=fopen(filename,"r");
    char linie[1024];
/*citesc prima linie pentru a deteremina nr de coloane*/
    fgets(linie,sizeof(linie),f);

    int n_col=0;
    char temp[1024];
    strcpy(temp,linie);
    int count=0;
    char *p=strtok(temp," \n");
    while(p){
        count++;
        p=strtok(NULL," \n");
    }
    /* pe o linie: n_col valori+(n_col-1) costuri = 2*n_col-1 elemente */
    n_col=(count+1)/2;
/*nr liniile totale din fisier*/
    int total_linii=1;
    while(fgets(linie,sizeof(linie),f)){
        total_linii++;
    }
    /* total_linii = n_lin + (n_lin-1) = 2*n_lin - 1 */

    int n_lin=(total_linii+1)/2;

    h->linii=n_lin;
    h->coloane=n_col;

    h->valori=malloc(n_lin * sizeof(int *));
    for(int i=0; i < n_lin; i++){
        h->valori[i]=malloc(n_col * sizeof(int));
    }

    h->cost_linii=malloc(n_lin * sizeof(float *));
    for(int i=0; i<n_lin; i++){
        h->cost_linii[i]=malloc((n_col-1)* sizeof(float));
    }

        h->cost_coloane=malloc((n_lin-1) * sizeof(float *));
        for(int i=0; i<n_lin-1; i++){
            h->cost_coloane[i]=malloc(n_col * sizeof(float));
        }

        rewind(f);

        for(int i=0; i<total_linii; i++){
            fgets(linie,sizeof(linie),f);
            linie[strcspn(linie,"\n")]='\0';
            p=strtok(linie," ");

            if(i%2==0){
                int row = i/2;
                for(int j=0; j<n_col; j++){
                    h->valori[row][j]=atoi(p);
                    p=strtok(NULL," ");
                    if(j<n_col-1){
                        h->cost_linii[row][j]=atof(p);
                        p=strtok(NULL," ");
                    }
                }
            }
            else {
                /* linie cu costuri verticale */
                int row = i/2;
                for(int j=0; j<n_col;j++){
                    h->cost_coloane[row][j]=atof(p);
                    p=strtok(NULL," ");
                }
            }
        }
        fclose(f);
}

void scriere_test_1_2(const char *filename,harta *h){
    FILE *f=fopen(filename,"w");
    int n=h->linii;
    int m=h->coloane;

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(j>0){
                fprintf(f," ");
            }
            fprintf(f,"%d",h->valori[i][j]);
        }
        fprintf(f,"\n");
    }

    /*matricea costurlilor verticale*/
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < m; j++){
            if (j > 0) fprintf(f, " ");
            fprintf(f,"%.2f", h->cost_coloane[i][j]);
        }
        fprintf(f,"\n");
    }
    /*matricea costurilor orizontale*/
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m - 1; j++) {
            if (j > 0) fprintf(f, " ");
            fprintf(f, "%.2f", h->cost_linii[i][j]);
        }
        fprintf(f,"\n");
    }
    fclose(f);
}


void free_harta(harta *h){
    for (int i = 0; i < h->linii; i++) {
        free(h->valori[i]);
        free(h->cost_linii[i]);
    }
    for (int i = 0; i < h->linii - 1; i++)
        free(h->cost_coloane[i]);
    free(h->valori);
    free(h->cost_linii);
    free(h->cost_coloane);
}

void init_stack(stack *s){
    s->top=NULL;
}

void push(stack *s,misiune m){
    stackNode *nou=malloc(sizeof(stackNode));
    nou->data=m;
    nou->next=s->top;
    s->top=nou;
}

void citire_misiuni(const char *filename, stack *s){
    FILE *f=fopen(filename,"r");
    char linie[256];

    fgets(linie,sizeof(linie),f); /*sare peste header*/

    while(fgets(linie,sizeof(linie),f)){
        linie[strcspn(linie,"\n")]='\0';
        misiune m;

        char *p=strtok(linie,";");

        /*preiau coordonatele in variabilele specifice*/
        m.xi=atoi(p);
        p=strtok(NULL,";");

        m.yi=atoi(p);
        p=strtok(NULL,";");

        m.xf=atoi(p);
        p=strtok(NULL,";");

        m.yf=atoi(p);

        push(s,m);
    }
    fclose(f);
}

void scriere_test_1_3(const char *filename, stack *s){
    FILE *f=fopen(filename,"w");
    fprintf(f,"Xi;Yi;Xf;Yf\n");

    stackNode *curr=s->top;
    while(curr){
        fprintf(f,"%d;%d;%d;%d",curr->data.xi,curr->data.yi,curr->data.xf,curr->data.yf);
        if(curr->next){
        fprintf(f,"\n");
        }
        curr=curr->next;
    }
    fclose(f);
}

void free_stack(stack *s){
    stackNode *curr=s->top;
    while(curr){
        stackNode *temp=curr;
        curr=curr->next;
        free(temp);
    }
}
/*creez un nod jucator cu label J<id>*/
BSTNode *createNode(jucator j){
        BSTNode *nou=malloc(sizeof(BSTNode));
        nou->data=j;
        nou->is_fight=0;
        nou->label = malloc(20);
        sprintf(nou->label, "J%d", j.id);
        nou->left=0;
        nou->right=0;
        return nou;
}
/*inserez jucatorul in BST dupa energie
jucatorii cu energie mai mica merg in stanga, iar cei cu energie mai mare sau egala merg in dreapta*/

BSTNode *insert_BST(BSTNode *root, jucator j){
    if(root == NULL){
        return createNode(j);
    }
    if(j.energie<root->data.energie){
        root->left=insert_BST(root->left,j);
    }
    else{
        root->right=insert_BST(root->right,j);
    }
    return root;
}
/*creez nod de lupta ce conecteaza doi copii*/
BSTNode *create_fight_node(BSTNode *left, BSTNode *right){
    BSTNode *fight=malloc(sizeof(BSTNode));
    fight->left=left;
    fight->right=right;
    fight->is_fight=1;
    /*noul label al nodului de lupta este format din concatenarea labelurilor celor doi copii*/
    fight->label = malloc(strlen(left->label) + strlen(right->label) + 1);
    sprintf(fight->label, "%s%s", left->label, right->label);
    fight->data.energie=0;
    return fight;
}

BSTNode *convert_to_contest(BSTNode *root){

    if(root==NULL){
        return NULL;
    }

    if(root->left == NULL && root->right == NULL){
        return root;
    } //frunzele raman frunze
    /*convertesc recursiv subarborii*/
    root->left=convert_to_contest(root->left);
    root->right=convert_to_contest(root->right);

    //cazul 1 - ambii copii exista

    if(root->left != NULL && root->right != NULL){
        BSTNode *parent_copy=createNode(root->data);
        parent_copy->is_fight=root->is_fight;
        free(parent_copy->label);
        parent_copy->label = malloc(strlen(root->label) + 1);
        strcpy(parent_copy->label,root->label);

        /*stanga se lupta cu parintele,apoi rezultatul cu dreapta*/

        BSTNode *left_fight=create_fight_node(root->left,parent_copy);
        BSTNode *top_fight=create_fight_node(left_fight,root->right);
        free(root->label);
        free(root);
        return top_fight;
    }

    //cazul 2 - exista doar copil stang

    if(root->left != NULL){
        BSTNode *parent_copy=createNode(root->data);
        parent_copy->is_fight=root->is_fight;
        free(parent_copy->label);
        parent_copy->label = malloc(strlen(root->label) + 1);
        strcpy(parent_copy->label, root->label);

        BSTNode *fight=create_fight_node(root->left,parent_copy);
        free(root->label);
        free(root);
        return fight;
    }

    //cazul 3 - doar copil drept

        BSTNode *parent_copy=createNode(root->data);
        parent_copy->is_fight=root->is_fight;
        free(parent_copy->label);
        parent_copy->label = malloc(strlen(root->label) + 1);
        strcpy(parent_copy->label,root->label);

        BSTNode *fight=create_fight_node(parent_copy,root->right);
        free(root->label);
        free(root);
        return fight;
}

/*parcurgerea in postordine-afiseaza labelul fiecrui nod*/
void postorder(BSTNode *root, FILE *f){
    if(root == NULL){
        return;
    }

    postorder(root->left,f);
    postorder(root->right,f);
    fprintf(f,"%s\n",root->label);

}

void free_tree(BSTNode *root){
    if(root==NULL){
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root->label);
    free(root);
}
/*aflu cea mai mare putere a lui 2 astfel incat 2^k<n , unde k retine valoarea puterii lui doi*/
int find_k(int n){
    int k=1;
    while(k*2<=n){
        k*=2;
    }
    return k;
}
/*functie de comp descrescatoare pt qsort*/
int cmp_desc(const void *a, const void *b) {
    jucator *ja = (jucator *)a;
    jucator *jb = (jucator *)b;
    if (jb->energie > ja->energie) return 1;
    if (jb->energie < ja->energie) return -1;
    return 0;
}

/*functia simuleaza luptele si colecteaza pierzatorii
se respecta regula : jucatorul cu energia mai mare trece mai departe sau cel din stanga in cazul in care au nergii egale
energia devine dupa formula max dintre cei doi + modulul diferentei energiilor acestora
iar pierzatorii sunt salvati in results
traversarea in postordine asigura ca luptele de jos sunt efectuate primele */

void fight_and_collect(BSTNode *root,jucator *results,int *count){
    if(root == NULL){
        return;
    }
    if(root->is_fight == 0){
        return;
    }
    fight_and_collect(root->left,results,count);
    fight_and_collect(root->right,results,count);

    float e1=root->left->data.energie;
    float e2=root->right->data.energie;

    if(e1>=e2){
        results[*count]=root->right->data; //pierzatorul
        root->data=root->left->data; //castigatorul
        root->data.energie=e1+(e1-e2);
    }
    else{
        results[*count]=root->left->data;
        root->data=root->right->data;
        root->data.energie=e2+(e2-e1);
    }

    (*count)++;
    root->is_fight=0;
}

void scriere_test_2_2(const char *filename,BSTNode *contest,int n){

    int k = find_k(n);

    jucator *all=malloc(n * sizeof(jucator));
    int count = 0;

    fight_and_collect(contest,all,&count);
    all[count]=contest->data;
    count++;

    qsort(all,count,sizeof(jucator),cmp_desc);

    FILE *f=fopen(filename,"w");
    for(int i=0; i<k; i++){
        fprintf(f,"J%d %.2f\n",all[i].id,all[i].energie);
    }
    fclose(f);
    free(all);
}
/*creez heap cu capacitate initiala data*/
MinHeap *create_heap(int capacity) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->data = malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_heap(HeapNode *a, HeapNode *b){
    HeapNode temp=*a;
    *a=*b;
    *b=temp;
}
/*adaug un nod in heap la final si urca apoi la pozitia sa corecta prin comparatii cu parintii sai sau se realoca spatiu in cazul in care heapul e plin*/
void heap_push(MinHeap *heap, HeapNode node){
    if(heap->size >= heap->capacity){
        heap->capacity *= 2;
        heap->data = realloc(heap->data, heap->capacity * sizeof(HeapNode));
    }
    heap->data[heap->size]=node;
    int i = heap->size;
    heap->size++;
    while(i>0){
        int parent=(i-1)/2;
        if(heap->data[i].f < heap->data[parent].f){
            swap_heap(&heap->data[i], &heap->data[parent]);
            i = parent;
        }
        else return;
    }
}
/*extrag elementul minim din heap , iar ultimul element trece pe poz 0, apoi coboara comparandu se cu copii sai*/
HeapNode heap_pop(MinHeap *heap) {
    HeapNode min = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size && heap->data[left].f < heap->data[smallest].f)
            smallest = left;
        if (right < heap->size && heap->data[right].f < heap->data[smallest].f)
            smallest = right;
        if (smallest != i) {
            swap_heap(&heap->data[i], &heap->data[smallest]);
            i = smallest;
        } else break;
    }
    return min;
}

void free_heap(MinHeap *heap) {
    free(heap->data);
    free(heap);
}

float find_min_cost(harta *h) {
    float min = h->cost_linii[0][0];
    for (int i = 0; i < h->linii; i++) {
        for (int j = 0; j < h->coloane - 1; j++) {
            if (h->cost_linii[i][j] < min)
                min = h->cost_linii[i][j];
        }
    }
    for (int i = 0; i < h->linii - 1; i++) {
        for (int j = 0; j < h->coloane; j++) {
            if (h->cost_coloane[i][j] < min)
                min = h->cost_coloane[i][j];
        }
    }
    return min;
}

/*returneaza costul de deplasare dintre doua casute
deplsare orizontala pe cost_linii si depl verticala pe cost_coloane*/
float get_cost(harta *h, int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        int min_y = (y1 < y2) ? y1 : y2;
        return h->cost_linii[x1][min_y];
    }
    int min_x = (x1 < x2) ? x1 : x2;
    return h->cost_coloane[min_x][y1];
}
/*la fiecare pas extrag nodul cu f minim din heap, explorez cei 4 vecini si actualizez costurile
 daca se gaseste un drum mai scurt.
 daca energia nu este suficienta - energie-g<=0 atunci acel drum este exclus
*/
void astar(harta *h, jucator *j, misiune *m, FILE *f) {
    int n = h->linii;
    int col = h->coloane;
    /*verific limitele*/
    if (m->xi < 0 || m->xi >= n || m->yi < 0 || m->yi >= col ||
        m->xf < 0 || m->xf >= n || m->yf < 0 || m->yf >= col) {
        fprintf(f, "J%d: Nu s-a putut efectua misiunea\n", j->id);
        return;
    }
    /*verific obstacolele la sursa/destinatie*/
    if (h->valori[m->xi][m->yi] == 1 || h->valori[m->xf][m->yf] == 1) {
        fprintf(f, "J%d: Nu s-a putut efectua misiunea\n", j->id);
        return;
    }

    float min_cost = find_min_cost(h);

    float **g = malloc(n * sizeof(float *));
    int **visited = malloc(n * sizeof(int *));
    int **prev_x = malloc(n * sizeof(int *));
    int **prev_y = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        g[i] = malloc(col * sizeof(float));
        visited[i] = malloc(col * sizeof(int));
        prev_x[i] = malloc(col * sizeof(int));
        prev_y[i] = malloc(col * sizeof(int));
        for (int jj = 0; jj < col; jj++) {
            g[i][jj] = 1e9; /*cost initial infinit*/
            visited[i][jj] = 0;
            prev_x[i][jj] = -1;
            prev_y[i][jj] = -1;
        }
    }

    MinHeap *heap = create_heap(n * col);
    /*initializez cu nodul sursa*/
    g[m->xi][m->yi] = 0;
    float h_val = abs(m->xi - m->xf) + abs(m->yi - m->yf);
    HeapNode start = {m->xi, m->yi, min_cost * h_val, 0};
    heap_push(heap, start);

    int found = 0;
    /*cele 4 directii sus jos stanga dreapta*/
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (heap->size > 0) {
        HeapNode u = heap_pop(heap);

        if (u.x == m->xf && u.y == m->yf) {
            found = 1;
            break; /*am ajuns la destinatie*/
        }
        /*nod deja procesat*/
        if (visited[u.x][u.y]) continue;
        visited[u.x][u.y] = 1;
        /*vizitez vecinii sai*/
        for (int d = 0; d < 4; d++) {
            int nx = u.x + dx[d];
            int ny = u.y + dy[d];

            if (nx < 0 || nx >= n || ny < 0 || ny >= col) continue;
            if (h->valori[nx][ny] == 1) continue;
            if (visited[nx][ny]) continue;

            float cost = get_cost(h, u.x, u.y, nx, ny);
            float new_g = g[u.x][u.y] + cost;
            /* jucatorul nu mai are energie suficienta */
            if (j->energie - new_g <= 0) continue;
            /*am gsit un drum mai scurt catre (nx,ny)*/
            if (new_g < g[nx][ny]) {
                g[nx][ny] = new_g;
                float heur = abs(nx - m->xf) + abs(ny - m->yf);
                float f_val = new_g + min_cost * heur;

                prev_x[nx][ny] = u.x;
                prev_y[nx][ny] = u.y;

                HeapNode next = {nx, ny, f_val, new_g};
                heap_push(heap, next);
            }
        }
    }

    if (found) {
        /*reconstruiesc calea de la destinatie la sursa*/
        int path_x[1000], path_y[1000];
        int len = 0;
        int cx = m->xf, cy = m->yf;
        while (cx != -1 && cy != -1) {
            path_x[len] = cx;
            path_y[len] = cy;
            len++;
            int px = prev_x[cx][cy];
            int py = prev_y[cx][cy];
            cx = px;
            cy = py;
        }

        float energie_ramasa = j->energie - g[m->xf][m->yf];
        fprintf(f, "J%d - %.2f:", j->id, energie_ramasa);
        /*afisez calea destinatie-sursa in ordine inversa*/
        for (int i = len - 1; i >= 0; i--) {
            if (i < len - 1) fprintf(f, " --");
            fprintf(f, " [%d, %d]", path_x[i], path_y[i]);
        }
        fprintf(f, "\n");
    } else {
        fprintf(f, "J%d: Nu s-a putut efectua misiunea\n", j->id);
    }

    for (int i = 0; i < n; i++) {
        free(g[i]);
        free(visited[i]);
        free(prev_x[i]);
        free(prev_y[i]);
    }
    free(g);
    free(visited);
    free(prev_x);
    free(prev_y);
    free_heap(heap);
}
/*pt fiecare jucator in ordine descresc a punctajului se atribuie o misiune scoasa din stiva si se ruleaza algoritmul astar*/
void scriere_test_3(const char *filename, jucator *winners, int k, stack *s, harta *h) {
    FILE *f = fopen(filename, "w");

    stackNode *curr_mis = s->top;
    for (int i = 0; i < k && curr_mis != NULL; i++) {
        astar(h, &winners[i], &curr_mis->data, f);
        curr_mis = curr_mis->next;
    }

    fclose(f);
}
