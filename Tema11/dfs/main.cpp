#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *      Am implementat algoritmul de cautare in adancime, de sortare topologica si algoritmul Tarjan pentru componente tare-conexe.
 *  Am creat 4 structuri, dintre care 2 structuri de date: Lista simplu inlantuita si stiva, pentru structura Node, care retine
 *  valori/informatii despre fiecare varf. Fiecare varf are o lista de adiacenta(reprezentata printr-un vector de Node-uri), un pointer
 *  la parinte,valoare varf,nr_vecini,culoare,timp de descoperire si finalizare ptr. dfs, iar pentru tarjan lowlink, index, daca e pus pe stiva,
 *  nr comp. Structura de graf contine un vector de Node-uri si nr de noduri.
 *      La DFS setam timpul de descoperire si culoarea varfului curent la descoperit(GREY), apoi parcurgem toti vecinii varfului curent
 *  si apelam recursiv DFS_Visit pe orice varf nedescoperit(ALB). Daca varful este deja descoperit(GRI), inseamna ca exista un ciclu
 *  si setam ciclu=1. Dupa ce sunt vizitate toate varfurile vecinului,se marcheaza varful curent ca terminat(NEGRU) si timpul de terminare
 *  si adaugam nodul in lista pentru sortarea topologica.
 *      La sortarea topologica,daca ciclu=1 dupa dfs, atunci nu se poate efectua o sortare topologica, daca ciclu=0, printam
 *  lista de node-uri, obtinuta din dfs, in ordinea finalizarii.
 *      La Tarjan, se foloseste ideea bfs-ului, setam indexul si lowlink ul nodului cu indexul, incrementam index
 *  apoi mergem pe toti vecinii nodului, apoi actualizeaza u->lowLink la valoarea minima dintre u->lowLink
 *  si v->lowLink. In caz contrar, daca v->onStiva este adevarat, se actualizeaza u->lowLink pentru a fi minimul
 *  dintre u->lowLink si v->index. Daca u->lowLink este egal cu u->index, inseamna ca nodul curent este radacina unei
 *  componente puternic conexe. Scoatem elementele din stiva pana cand elementul curent este gasit.
 *      Complexitatea este O(V+E), unde v=nr_varfuri, iar E=nr_muchii.
 *
*/
Profiler prof("DFS_grafice");

typedef struct Lista{
   int data;
   Lista* next;
}Lista;

typedef struct Node{
    int data;
    Node** liste;
    Node* parinte;
    int nr_vecini;
    int timp_descoperire;
    int timp_finalizare;
    int culoare;

    int lowLink;
    int index;
    int onStiva;
    int comp;
}Node;

typedef struct Graf{
    int nr_varfuri;
    Node** varfuri;
}Graf;

typedef struct Stiva {
    int data[200];
    int top;

}Stiva;

enum{WHITE,BLACK,GREY};

Node* creareNode(int data, int n){
    ///alocam node de tipul structurii
    Node* node = (Node*) malloc(sizeof(Node));
    ///initializari
    node->data = data;
    node->parinte = NULL;
    node->culoare = WHITE;
    node->liste = (Node**) malloc(n*sizeof(Node*));
    node->nr_vecini = 0;
    node->timp_descoperire = 0;
    node->timp_finalizare =0;

    node->lowLink=-1;
    node->index=-1;
    node->onStiva=0;
    node->comp=0;

    return node;
}
Graf* creareGraf(int n){
    ///alocam graful de tipul structurii
    Graf* graf = (Graf*) malloc(sizeof(Graf));
    graf->nr_varfuri = n;
    graf->varfuri = (Node**) malloc((graf->nr_varfuri)*sizeof(Node*));
    ///cream varfurile de tipul Node
    for(int i=1;i<=graf->nr_varfuri;i++){
        graf->varfuri[i] = creareNode(i,graf->nr_varfuri);
    }
    return graf;
}
Lista* creareNodeLista(Node* node){
    ///alocam lista si o initializam
    Lista* nodeLista = (Lista*) malloc(sizeof(Lista));
    nodeLista->data = node->data;
    nodeLista->next = NULL;
    return nodeLista;
}

void muchie(Graf* graf, int i,int j){

    ///pentru varful i crestem numarul de vecini ca sa putem crea muchia
    graf->varfuri[i]->nr_vecini+=1;
    int nr = graf->varfuri[i]->nr_vecini;
    ///adaugam in vectorul de lista de adiacenta varful j
    graf->varfuri[i]->liste[nr] = graf->varfuri[j];
    for(int m=0; m<graf->varfuri[i]->nr_vecini;m++){
        for(int k=m+1; k<-graf->varfuri[i]->nr_vecini;k++){
            ///ordonam lista de adiacenta
            if(graf->varfuri[i]->liste[m]->data > graf->varfuri[i]->liste[k]->data)
            {
                Node *node=graf->varfuri[i]->liste[m];
                graf->varfuri[i]->liste[m]=graf->varfuri[i]->liste[k];
                graf->varfuri[i]->liste[k]=node;
            }
        }
    }
}
Lista* add_lista_first(Node* node, Lista* lista){
    if(lista == NULL) {
        ///daca lista este nula, cream lista
        lista = creareNodeLista(node);
        return lista;
    }
    else {
        ///cream un node de tipul listei
        Lista* nodeList = creareNodeLista(node);
        ///adaugam la inceputul listei
        nodeList->next = lista;
        lista = nodeList;
        return lista;
    }
}

void DFS_Visit(Graf* graf, Node* u, int* timp, int* ciclu, Lista** lista, int bfs, Operation* operatie){

    (*timp)+=1;
    operatie->count(2);
    ///fixam timpul de descoperire si culoarea ca vizitat
    u->timp_descoperire = (*timp);
    u->culoare = GREY;

    if(bfs) {
        printf("%d ", u->data);
    }
    for(int i=1; i <= u->nr_vecini;i++){
        operatie->count();
        ///luam vecinii acelui varf
        Node* v = u->liste[i];
         if(v->culoare == WHITE){
             ///daca este un vecin nevizitat, mergem cu dfs pe el
             v->parinte = u;
             DFS_Visit(graf,v,timp,ciclu, lista,bfs,operatie);
         }
         else
             if(v->culoare == GREY){
             ///are ciclu, deci nu poate avea sortare topologica
             *ciclu = 1;
         }
    }

    operatie->count(2);
    ///daca am ajuns aici, ne-am reintors in varf, deci este revizitat
    u->culoare = BLACK;
    (*timp)+=1;
    ///fixam si timpul de finalizare
    u->timp_finalizare = (*timp);
    ///adaugam node-ul in lista de sortare topologica
    *lista = add_lista_first(u,*lista) ;
}

void DFS(Graf* graf, Lista** lista,Operation* operatie){
    int time=0;
    ///de la initializarea pe varfuri
    operatie->count(3*graf->nr_varfuri);
    for(int i=1;i<=graf->nr_varfuri;i++){
        ///pentru fiecare varf care este nevizitat, aplicam dfs
        if(graf->varfuri[i]->culoare == WHITE){
            int ciclu = 0;
            DFS_Visit(graf,graf->varfuri[i],&time, &ciclu,lista,1,operatie);
        }
    }
}
void sortarea_topologica(Graf* graf, Lista* lista) {
Operation op = prof.createOperation("Nimic",graf->nr_varfuri);
    int ciclu = 0;

    for (int i = 1; i <= graf->nr_varfuri; i++) {
        ///aplicam dfs pe fiecare varf
        if (graf->varfuri[i]->culoare == WHITE) {
            int time = 0;
            DFS_Visit(graf, graf->varfuri[i], &time, &ciclu,&lista,0,&op);
        }
    }
    ///daca este ciclu, nu este posibila o sortare topologica
    if (ciclu == 1) {
        printf("\nGraful are ciclu, deci sortarea topologica nu este posibila.\n");
    }
    else {
        ///in caz contrar, printam lista in care se afla varfurile in ordine inversa a vizitarii lor
        printf("\nSortarea topologica: ");
        while (lista != NULL) {
            printf("%d ", lista->data);
            lista = lista->next;
        }
        printf("\n");
    }
}

void printGraf(Graf* graf){
    ///printam graful sub forma listelor de adiacenta
    for(int i=1;i<=graf->nr_varfuri;i++){
        printf("\n%d:",i);
        for(int j=1;j<=graf->varfuri[i]->nr_vecini;j++){
            printf(" %d",graf->varfuri[i]->liste[j]->data);
        }
     }
}

Stiva* creaza_stiva() {
    ///cream stiva si o initializam
    Stiva* stiva = (Stiva*)calloc(1,sizeof(Stiva));
    stiva->top = -1;
    return stiva;
}

void push(Stiva* s, int element) {
    ///punem pe stiva elementul
    s->top += 1;
    s->data[s->top] = element;
}

int pop(Stiva* s) {
    ///in cazul in care nu mai avem ce scoate de pe stiva
    if (s->top == -1) {
        return -1;
    }
    ///
    int element = s->data[s->top];
    s->top -= 1;
    return element;
}

int minim (int a, int b) {
    ///functie de minim
    if (a > b)
        return b;
     else
        return a;

}

void Tarjan_TareConexe(Node* u,Stiva* stiva, int* nr_comp, int* index) {

    u->index = u->lowLink = *index;
    *index+=1;
    ///punem pe stiva elementul
    push(stiva, u->data);
    u->onStiva = 1;

    for (int i = 1; i <= u->nr_vecini; i++) {
        ///pentru fiecare vecin al varfului
        Node* v = u->liste[i];
        if (v->index == -1) {
            ///aplicam tarjan recursiv
            Tarjan_TareConexe(v,stiva, nr_comp, index);
            ///restabilim lowlink-ul ca minimul dintre lowlink ul lui u si a lui v
            u->lowLink = minim(u->lowLink, v->lowLink);
        }
        else
            if (v->onStiva) {
                ///daca este pe stiva, restabilim lowlink ca minimul dintre lowlink de u si indexul lui v
            u->lowLink = minim(u->lowLink, v->index);
        }
    }
    /// trebuie sa scoatem de pe stiva, in cazul in care lowlink si index sunt egale
    if (u->lowLink == u->index) {
       Node* v = (Node*) malloc(sizeof(Node));
       *nr_comp+=1;

        do {
            v->data = pop(stiva);
            v->onStiva = 0;
            v->comp = *nr_comp;
        }while (u->data != v->data);
    }
}

void Tarjan(Graf* graf) {

    int nr_componente = 0, index = 0;
    ///cream stiva
    Stiva* stiva= creaza_stiva();
    if (graf == NULL) {
        return;
    }
    for (int i = 1; i <= graf->nr_varfuri; i++) {
        if (graf->varfuri[i]->index == -1) {
            ///aplicam tarjan pentru fiecare vecin
            Tarjan_TareConexe(graf->varfuri[i],stiva,&nr_componente,&index);
        }
    }
    printf("Nr. componente: %d",nr_componente);
}

void demo_BFS(){
    Operation opp = prof.createOperation("op",10);
    Lista* lista = NULL;

    printf("Primul graf:");

    Graf* graf = creareGraf(6);
    muchie(graf,1,2);
    muchie(graf,2,3);
    muchie(graf,3,4);
    muchie(graf,4,2);
    muchie(graf,1,4);
    muchie(graf,5,4);
    muchie(graf,5,6);
    muchie(graf,6,6);
    printGraf(graf);
    printf("\nDFS:\n");
    DFS(graf,&lista,&opp);

    printf("\n\nAl doilea graf:");

    Graf* graf2 = creareGraf(4);
    Lista* lista2 = NULL;
    muchie(graf2,1,2);
    muchie(graf2,1,3);
    muchie(graf2,2,3);
    muchie(graf2,3,1);
    muchie(graf2,3,4);
    muchie(graf2,4,4);
    printGraf(graf2);
    printf("\nDFS:\n");
    DFS(graf2, &lista2,&opp);


    printf("\n\nAl treilea graf:");

    Graf* graf3 = creareGraf(6);
    Lista* lista3 = NULL;
    muchie(graf3, 1, 3);
    muchie(graf3, 2, 3);
    muchie(graf3, 3, 4);
    muchie(graf3, 4, 5);
    muchie(graf3, 5, 6);
    printGraf(graf3);
    printf("\nDFS:\n");
    DFS(graf3,&lista3,&opp);

}
void demo_sortare_topologica(){

    Lista* lista = NULL;

    printf("Primul graf:");
    Graf* graf = creareGraf(6);
    muchie(graf,1,2);
    muchie(graf,2,3);
    muchie(graf,3,4);
    muchie(graf,4,2);
    muchie(graf,1,4);
    muchie(graf,5,4);
    muchie(graf,5,6);
    muchie(graf,6,6);
    printGraf(graf);
    sortarea_topologica(graf, lista);


    printf("\n\nAl doilea graf:");
    Graf* graf2 = creareGraf(4);
    Lista* lista2 = NULL;
    muchie(graf2,1,2);
    muchie(graf2,1,3);
    muchie(graf2,2,3);
    muchie(graf2,3,1);
    muchie(graf2,3,4);
    muchie(graf2,4,4);
    printGraf(graf2);
    sortarea_topologica(graf2, lista2);


    printf("\n\nAl treilea graf:");
    Graf* graf3 = creareGraf(6);
    Lista* lista3 = NULL;
    muchie(graf3, 1, 3);
    muchie(graf3, 2, 3);
    muchie(graf3, 3, 4);
    muchie(graf3, 4, 5);
    muchie(graf3, 5, 6);
    printGraf(graf3);
    sortarea_topologica(graf3, lista3);

}
void demo_Tarjan(){

    printf("Primul graf:");
    Graf* graf = creareGraf(6);
    muchie(graf,1,2);
    muchie(graf,2,3);
    muchie(graf,3,4);
    muchie(graf,4,2);
    muchie(graf,1,4);
    muchie(graf,5,4);
    muchie(graf,5,6);
    muchie(graf,6,6);
    printGraf(graf);
    printf("\n");
    Tarjan(graf);


    printf("\n\nAl doilea graf:");
    Graf* graf2 = creareGraf(4);
    muchie(graf2,1,2);
    muchie(graf2,1,3);
    muchie(graf2,2,3);
    muchie(graf2,3,1);
    muchie(graf2,3,4);
    muchie(graf2,4,4);
    printGraf(graf2);
    printf("\n");
    Tarjan(graf2);


    printf("\n\nAl treilea graf:");
    Graf* graf3 = creareGraf(8);

    muchie(graf3, 1, 3);
    muchie(graf3, 2, 1);
    muchie(graf3, 3, 4);
    muchie(graf3, 2, 4);
    muchie(graf3, 3, 4);
    muchie(graf3, 3, 5);
    muchie(graf3, 4, 6);
    muchie(graf3, 6, 4);
    muchie(graf3, 5, 6);
    muchie(graf3, 5, 7);
    muchie(graf3, 7, 5);
    muchie(graf3, 7, 8);
    muchie(graf3, 6, 8);


    printGraf(graf3);
    printf("\n");
    Tarjan(graf3);
}
void perf(){
    srand(time(0));

    for(int n = 1000; n <= 4500;n+=100){
        ///numar fix de varfuri, variem nr de muchii intre 1000 si 4500

        Operation operatie = prof.createOperation("DFS_nrFixVarfuri",n);
        Graf* graf = creareGraf(100);
        int nr = 0;
        do{
            ///doua varfuri random, intre 1 si 100
            int rand1=(rand()%(graf->nr_varfuri-1))+1;
            int rand2=(rand()%(graf->nr_varfuri-1))+1;


            if(rand1!=rand2) {
                    ///presupunem ca nu exista deja o muchie identica cu cea introdusa si cautam
                    int ok = 0;
                    for (int i = 0; i < graf->varfuri[rand1]->nr_vecini; i++) {
                        if (graf->varfuri[rand1]->liste[i] == graf->varfuri[rand2])
                            ok = 1;
                        else
                            ok = 0;

                    }
                    ///in cazul in care nu am gasit o muchie formata din cele doua varfuri, cream o muchie
                    if (ok == 0) {
                        muchie(graf, rand1, rand2);
                        nr++;
                    }
                }
       }while(nr!=n);
        Lista* lista = NULL;
        DFS(graf, &lista,&operatie);
    }
    for(int n = 100; n <= 200;n+=10){
        ///numar fix de muchii 4500 si variem numarul de varfuri
        Operation operatie2 = prof.createOperation("DFS_nrFixMuchii",n);
        Graf* graf = creareGraf(n);
        int nr = 0;
        do{

            int rand1=(rand()%(graf->nr_varfuri-1))+1;
            int rand2=(rand()%(graf->nr_varfuri-1))+1;

            if(rand1!=rand2) {
            ///presupunem ca nu exista deja o muchie identica cu cea introdusa si cautam
                int ok = 0;
                for (int i = 0; i < graf->varfuri[rand1]->nr_vecini; i++) {
                    if (graf->varfuri[rand1]->liste[i] == graf->varfuri[rand2])
                        ok = 1;
                    else
                        ok = 0;

                }
            ///in cazul in care nu am gasit o muchie formata din cele doua varfuri, cream o muchie
                if (ok == 0) {
                    muchie(graf, rand1, rand2);
                    nr++;
                }
            }
        }while(nr!=4500);
        Lista* lista2 = NULL;
        DFS(graf, &lista2,&operatie2);
    }
    prof.showReport();
}
int main() {

    //demo_BFS();
    //demo_sortare_topologica();
   demo_Tarjan();
    //perf();

    return 0;
}