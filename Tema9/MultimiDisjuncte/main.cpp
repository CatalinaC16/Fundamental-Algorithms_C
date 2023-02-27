#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"
#define MAX_SIZE 10000
/**
 * Ciochina Catalina-Andreea
 * Semigrupa1, grupa 30228
 *     Am implementat operatiile de baza pe multimi disjuncte: MAKE_SET(x) care creaza o multime noua, care contine elementul x si
 *  returneaza Node ul creat. Am facut o structura de tipul Node, care contine o valoare, pointer catre node ul parinte/ primul
 *  introdus in multime si rang ul acelui node. Operatie de FIND_SET(x) unde cautam node ul si returnam parintele initial.
 *  Operatia de UNION(x,y) foloseste functia de LINK, unde cautam in care multime se afla node ul.
 *     Am creat graful cu ajutorul vectorilor de muchii si varfuri. Pentru graf am facut o structura care contine nr de varfuri si
 *  de muchii si pointer la vect de muchii, o muchie are un node stanga, un node dreapta si o pondere,un cost si am trimis graful
 *  la Kruskal.
 *     In Kruskal am creat graful de acoperire minima, am aplicat quicksort pe muchii si parcurgem toate muchiile.
 * Eliminam o muchie de cost minim -> daca acea muchie conecteaza doi arbori distincti, atunci adaugam muchia si combinam
 * cei doi arbori in unul ->daca nu conecteaza doi arbori distincti, ignoram muchia.
 *     La quicksort am ales ca pivot ultimul element si  elementele mai mici decat pivot se plaseaza in stanga,
 * iar cele mai mari in dreapta pivotului, j<= pivot, inc i si swap la elem de pe poz i si j, proces repetat pana cand j ajunge
 * la pozitia pivotului, in final pivotul se pune pe pozitia i+1 (partitionare). Dupa fiecare partitie, apelam recursiv quicksort pe
 * cele 2 subsiruri obtinute la fiecare pas, repetand intregul procedeu pana cand start >= end. Worst Case O(n^2)
 * -> sir sortat. Best si Avg O(n*logn).
 *     Complexitatea algoritmului Kruskal este O(nr_muchii * log(nr_muchii)).
 * O(nr_varfuri) la crearea multimilor ptr fiecare varf, O(nr_muchii * log(nr_muchii) la sortarea muchiilor(quicksort)
 * si pentru fiecare muchie, O(nr_muchii) pe for si O(alfa* nr_muchii) in interior. MAKE_SET si FIND_SET iau O(1), iar
 * UNION  O(m) sau O(inaltime arbore).
 */

Profiler prof("MultimiDisjuncte");

typedef struct Node{
    int x;
    Node* parinte;
    int rank;
}Node;

typedef struct Muchie{
    Node* stanga;
    Node* dreapta;
    int cost;
}Muchie;

typedef struct Graf{
    int nr_muchii;
    int nr_varfuri;
    Muchie* muchii;
}Graf;

void schimba_elem(Muchie *elem1, Muchie *elem2) {
    ///schimb de elemente
    Muchie aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

int partition(Muchie* muchii, int start, int end) {
   ///stabilim ca pivot ultimul cost
    int pivot = muchii[end].cost;
    int i = start-1;

    for (int j = start; j<= end - 1; j++) {
        ///sortam in functie de cost
        if (muchii[j].cost <= pivot) {
            i++;
            ///schimbam
            schimba_elem(muchii + j, muchii + i);
        }
    }
    schimba_elem(muchii + end, muchii + i + 1);
    i+=1;
    return i;
}

void QuickSort(Muchie* muchii, int start, int end) {
 ///am depasit limitele
    if (start >= end)
        return;
    else if (start < end) {
        ///stabilim pivot
        int piv_index = partition(muchii, start, end);
        ///mergem pe subsir stanga
        QuickSort(muchii, start, piv_index - 1);
        ///mergem pe subsir dreapta
        QuickSort(muchii, piv_index + 1, end);
    }
}

Node* MAKE_SET(int x){

    ///cream node ul
    Node* node = (Node*)calloc(1,sizeof(Node));
    node->x = x;
    ///ii asignam ca parinte pe el insusi
    node->parinte = node;
    node->rank =0;
    return node;
}

Graf* MAKE_GRAPH(int nr_muchii,int nr_varfuri,Muchie* muchii){
    ///cream graf de tipul structurii
    Graf* graf = (Graf*) calloc(1,sizeof(Graf));
    ///asignam pointer la vectorul de muchii, trimis ca parametru
    graf->muchii = muchii;
    ///nr de muchii si varfuri
    graf->nr_muchii = nr_muchii;
    graf->nr_varfuri = nr_varfuri;
    return graf;
}

Node* FIND_SET(Node* x,Operation operatie){
    operatie.count();
    ///are parinte
    if(x->parinte != x)
    {
        operatie.count();
        ///cautam in continuare, recursiv parintele
        x->parinte = FIND_SET(x->parinte,operatie);
    }
    else
        return x->parinte;
    ///nu are parinte, este egal cu parintele
}

void LINK_NODES(Node* x,Node* y, Operation operatie){

    operatie.count();
    if(x->rank > y->rank)
    {
        ///daca rang ul este mai mare atunci parintele lui y devine x
        operatie.count();
        y->parinte =x;
    }
    else
    {
        ///daca nu parintele lui x devine y
        operatie.count();
        x->parinte =y;

        operatie.count();
        ///crestem rang ul lui y
        if(x->rank == y->rank){
            operatie.count();
            y->rank +=1;
        }
    }
}

void UNION(Node* x,Node* y, Operation operatie){
    LINK_NODES(FIND_SET(x,operatie), FIND_SET(y,operatie),operatie);
}

Muchie* MAKE_MUCHII(Node** varfuri,int* stanga,int* dreapta,int* cost,int nr_muchii,Operation operatie){
    ///cream vector de muchii
    Muchie* muchii = (Muchie*) calloc(nr_muchii, sizeof(Muchie));
    for(int i=0;i<nr_muchii;i++){

        ///gasim set ptr muchie stanga de i
        operatie.count();
        muchii[i].stanga = FIND_SET(varfuri[stanga[i]],operatie);
        ///gasim set ptr muchie dreapta de i
        operatie.count();
        muchii[i].dreapta = FIND_SET(varfuri[dreapta[i]],operatie);
        ///asignam si costul
        muchii[i].cost = cost[i];
    }
    return muchii;
}
Muchie* KRUSKAL(Graf* graf,Node** varfuri, int *nr, Operation operatie){

    int n= graf->nr_muchii;
    ///alocam arborele de acoperire minima
    Muchie* grafAcoperireMin =(Muchie*)calloc(n,sizeof(Muchie));

    ///aplicam quicksort pe muchii
    QuickSort(graf->muchii,0,n-1);

    for(int i = 0;i < n;i++){
        Muchie muchie = graf->muchii[i];
        Node* x = FIND_SET(varfuri[muchie.stanga->x],operatie);
        Node* y = FIND_SET(varfuri[muchie.dreapta->x],operatie);
        if(x != y){

            ///evitam creare ciclu
            grafAcoperireMin[*nr] = muchie;
            UNION(x,y,operatie);
            *nr+=1;
        }
    }
    return grafAcoperireMin;
}
void demoSET(){

    Node** arbore = (Node**) calloc(10,sizeof(Node*));
    int x[10]={0,1,2,3,4,5,6,7,8,9};
    int n = sizeof(x)/sizeof(*(x));

    Operation op = prof.createOperation("NuCont",n);
    printf("Am creat 10 multimi:\n");
    ///cream multimile
    for(int i = 0;i < n; i++){
        arbore[i] = MAKE_SET(x[i]);
        printf("%d->%d\n",arbore[i]->x, FIND_SET(arbore[i],op)->x);
    }
    ///reunim multimi
    UNION(arbore[2],arbore[9],op);
    UNION(arbore[3],arbore[7],op);
    UNION(arbore[4], arbore[8],op);
    UNION(arbore[1], arbore[6],op);
    UNION( arbore[0],arbore[5],op);

    printf("Am creat reuniuni:\n");
    ///afisam reuniunile create
    for(int i = 0;i < n; i++) {
        printf("%d->%d\n", arbore[i]->x, FIND_SET(arbore[i],op)->x);
    }
}

void demoKRUSKAL(){
    /*
        9
        14
        1 2 4
        2 8 8
        1 8 8
        8 9 7
        2 3 8
        3 9 2
        9 7 6
        7 6 2
        3 6 4
        3 4 7
        4 6 14
        4 5 9
        5 6 10
        8 7 1
        Arborele de acoperire minima: (8-7)1 -> (3-9)2 -> (7-6)2 -> (3-6)4 -> (1-2)4 -> (3-4)7 -> (2-3)8 -> (4-5)9

        5
        6
        1 2 3
        2 4 2
        1 3 7
        3 5 5
        3 4 2
        4 5 4
        Arborele de acoperire minima: (2-4)2 -> (3-4)2 -> (1-2)3 -> (4-5)4
     */
    int nr_varfuri, nr_muchii ,nr = 0;
    printf("Nr varfuri este:");
    scanf("%d",&nr_varfuri);
    printf("Nr muchii este:");
    scanf("%d",&nr_muchii);

    Operation op = prof.createOperation("NuCont",nr_varfuri);

    int* stanga, *dreapta,*cost;
    stanga = (int*) calloc(nr_muchii,sizeof(int));
    dreapta = (int*) calloc(nr_muchii,sizeof(int));
    cost = (int*) calloc(nr_muchii,sizeof(int));

    for(int i = 0;i < nr_muchii;i++){
        scanf("%d %d %d",stanga+i,dreapta+i,cost+i);
    }

    ///cream varfurile ca multimi independente
    Node** varfuri =  (Node**) calloc(nr_varfuri,sizeof(Node*)) ;
    for(int i=1;i<=nr_varfuri;i++){
        varfuri[i]= MAKE_SET(i);
    }
    ///cream muchiile
    Muchie* muchii= MAKE_MUCHII(varfuri,stanga,dreapta,cost,nr_muchii,op);
    ///facem graful, asignam valorile
    Graf* graf = MAKE_GRAPH(nr_muchii,nr_varfuri,muchii);
    ///trimitem la kruskal
    Muchie* arbore =KRUSKAL(graf,varfuri,&nr,op);
    ///am obtinut arborele de acoperire minima
    printf("Arborele de acoperire minima:\n");
    for (int i = 0; i < nr-1; i++) {
        printf("(%d-%d)%d -> ",arbore[i].stanga->x, arbore[i].dreapta->x,arbore[i].cost);
    }
    printf("(%d-%d)%d ",arbore[nr-1].stanga->x, arbore[nr-1].dreapta->x,arbore[nr-1].cost);
}

void operatii_perf(){

    srand(time(0));

    for(int size = 100; size <= MAX_SIZE; size += 100){

        Operation operatii = prof.createOperation("Kruskal_Operatii",size);
        ///multimi independente ptr fiecare varf
        Node** varfuri = (Node**) calloc(size,sizeof(Node*)) ;
        for(int i=1;i<=size;i++){
            varfuri[i]= MAKE_SET(i);
        }

        Muchie* muchii = (Muchie*)calloc(4*size,sizeof(Muchie));

        int* stanga = (int*) calloc(4*size,sizeof(int));
        int* dreapta = (int*) calloc(4*size,sizeof(int));
        int* cost = (int*) calloc(4*size,sizeof(int));

        FillRandomArray(stanga,4*size,1,size,false,UNSORTED);
        FillRandomArray(dreapta,4*size,1,size,false,UNSORTED);
        FillRandomArray(cost,4*size,1,size,false,UNSORTED);

        for(int i = 0; i < 4*size;i++){
            muchii[i].stanga = FIND_SET(varfuri[stanga[i]],operatii);
            muchii[i].dreapta = FIND_SET(varfuri[dreapta[i]],operatii);
            muchii[i].cost = cost[i];
        }

        int nr=0;
        Graf* graf = MAKE_GRAPH(4*size,size,muchii);
        Muchie* arbore = KRUSKAL(graf,varfuri,&nr,operatii);

    }
    prof.showReport();


}
int main(){
    ///alternativ,au lucruri comune
   // demoSET();
    demoKRUSKAL();
    //operatii_perf();
}
