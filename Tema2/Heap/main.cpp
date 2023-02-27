#include <stdio.h>
#include "Profiler.h"
/*
 * * * Ciochina Catalina-Andreea
 * * * * Grupa 30228- semigrupa 1
 *
 *  Am implementat metodele de construire a structurii de date Heap( Top-Down si Bottom-Up)  si algoritmul de heapsort.
 *
 * --------ANALIZA---------
 *
 * * * Implementarea algoritmilor * * *
 *
 *  La varianta de construire Top-down, avem un vector care are valorile care trebuie sa existe intr-un heap, la fiecare pas
 *  in vectorul a[], cresteam dimeniunea heap-ului, tot reprezentat ca array, inseram valoarea ca frunza si dupa restabilim
 *  ordinea elementelor in sus, pana la radacina, ca sa fie respectata proprietatea de max heap.
 *
 *  La varianta de construire bottom-up, pornim de la un heap, reprezentat ca un array. Pornim cu parcurgerea de la "nodurile"
 *  care nu sunt frunze(n/2-1 ca pornim de la 0 cu parcurgerea) si ne uitam in jos, la copii sai si in cazul in care
 *  nu respecta conditia de max heap, facem heapify( heapify the subtree).
 *
 * 1. Average Case == vector nesortat
 *     Complexitatea alg. de construire Top-Down este O(n*logn), iar Bottom-Up este mai eficient cu o complexitate liniara,
 *     ~=O(n).
 *
 * 2. Worst Case ==  vector sortat crescator
 *     Complexitatile sunt O(n*logn), insa si de aceasta data Bottom-Up este mai eficient decat Top-Down.
 *
 * Nu sunt algoritmi stabili, sunt algoritmi optimali. Complexitatea alg. de sortare este O(n*logn).
 *
 */
#define max_size 10000
#define step_size 100
#define teste 5

Profiler prof("heap");

void schimba_elem( int *elem1, int *elem2){
    int aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

void acelasi_vector(int *initial, int * final, int n){
    for(int i=0; i<n; i++){
        *(final+i)=*(initial+i);
    }
}

void printeaza_vector( int *a, int n){
    for(int i=0; i<n; i++){
        printf("%d ", *(a+i));
    }
    printf("\n");
}

int parinte( int i){
   return (i-1)/2;
}

/// parcurgem de la 0 la sizeA-1 deci copii lui a[i] sunt la pozitiile 2*i+1 si 2*i+2

int copil_stanga(int i){
    return 2*i+1;
}

int copil_drepta(int i){
    return 2*i+2;
}

/// heapify (recursiv) utilizat la metoda de construire bottom-up a heap-ului

void heapify( int *a, int n, int i,int *opComp, int *opAttr){

    int curent = i;
    int stg = copil_stanga(i);
    int dr = copil_drepta(i);

    if(stg < n){

        *opComp+=1;

            if( *(a+stg) > *(a+curent))
                curent = stg;
        }
    if(dr < n){

        *opComp+=1;

            if(*(a+dr) > *(a+curent)){
                curent = dr;
            }
    }
    ///inseamna ca nu este la locul bun si trebuie sa parcurgem de acolo in jos => heapify the subtree
    ///ca sa ii stabilim locul unde trebuie sa fie
    if(curent != i){

        *opAttr+=3;
        schimba_elem(a+i,a+curent);
        heapify(a,n,curent,opComp,opAttr);
    }

}

void heap_bottom_up(int *a, int n){
    Operation opComp = prof.createOperation("Bottom_up-comp",n);
    Operation opAttr = prof.createOperation("Bottom_up-attr",n);

    int co=0,atr=0;
    ///eliminam frunzele - nodurile care nu au copil drept si stang => (n/2)-1
    /// si facem heapify la fiecare element ramas

    for (int i = ((n/ 2) - 1); i >= 0; i--) {

        heapify(a, n, i,&co,&atr);

    }
    opAttr.count(atr);
    opComp.count(co);
}

void reconstruieste(int *heap, int index, int *opComp, int *opAttr){

    if(index < 1) return;
    else{

        int parinte_index = parinte(index);

        *opComp+=1;
        if (*(heap+parinte_index) < *(heap+index)) {

            *opAttr+=3;
            schimba_elem(heap+parinte_index, heap+index);
            ///mergem in sus in heap, pana la radacina
            index = parinte_index ;
            reconstruieste(heap, index, opComp, opAttr);
        }
        else return;
    }
}
void heap_construire(int *heap, int *dim, int element,int *opComp, int *opAttr){

    *dim+= 1;
    *opAttr+=1;
    heap[*dim] = element;
    int index = *dim;
    reconstruieste(heap, index, opComp, opAttr);
}

void heap_top_down(int *a, int *heap, int n ){
    Operation opComp = prof.createOperation("Top_down-comp",n);
    Operation opAttr = prof.createOperation("Top_down-attr",n);

    int co=0,atr=0;
    int dimensiune = -1;

    for (int i = 0; i < n; i++)
        heap_construire(heap, &dimensiune, *(a+i),&co, &atr);

    opAttr.count(atr);
    opComp.count(co);
}

//***HEAPSORT***//** bottom_up_heapsort
void heap_sort(int *a, int n){

    int op=0,atr=0;
    ///construim heap-ul bottom-up
    heap_bottom_up(a,n);
    ///sortam
    for(int i=n-1; i>=0; i--) {
        schimba_elem(a, a + i);
        heapify(a,i,0,&op,&atr);
    }
}

void demo(){
    int a[] = {10,25, 15 ,10 ,9 ,10 ,13 ,27};
    int size = sizeof(a) / sizeof(int);
    int heap[size], b[size],sort[size];

    acelasi_vector(a,b,size);
    acelasi_vector(a,sort, size);

    printf("INITIAL: ");
    printeaza_vector(a, size);

    printf("TOP-DOWN HEAP: ");
    heap_top_down(b,heap,size);
    ///lucram numai cu vectorul *heap
    printeaza_vector(heap, size);

    printf("BOTTOM-UP HEAP: ");
    heap_bottom_up(a, size);
    printeaza_vector(a, size);

    printf("HEAP_SORT: ");
    //heap_bottom_up(sort,size);
    heap_sort(sort,size);
    printeaza_vector(sort, size);

}
void Average_Case(){

  int a[max_size],b[max_size],heap[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, UNSORTED);
            acelasi_vector(a,b,n);
            heap_top_down(b,heap,n);
            heap_bottom_up(a,n);
          }
    }

    prof.divideValues("Top_down-attr",teste);
    prof.divideValues("Bottom_up-attr",teste);

    prof.divideValues("Top_down-comp",teste);
    prof.divideValues("Bottom_up-comp",teste);

    prof.addSeries("Top_down_Total","Top_down-attr","Top_down-comp");
    prof.addSeries("Bottom_up_Total","Bottom_up-attr","Bottom_up-comp");

    prof.createGroup("Average CASE - atribuiri","Top_down-attr","Bottom_up-attr");
    prof.createGroup("Average CASE - comparatii","Top_down-comp","Bottom_up-comp");
    prof.createGroup("Total__Average","Top_down_Total","Bottom_up_Total");
}
void Worst_Case(){

    int a[max_size],b[max_size],heap[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, ASCENDING);
            acelasi_vector(a,b,n);
            heap_top_down(b,heap,n);
            heap_bottom_up(a,n);
        }
    }

    prof.divideValues("Top_down-attr",teste);
    prof.divideValues("Bottom_up-attr",teste);

    prof.divideValues("Top_down-comp",teste);
    prof.divideValues("Bottom_up-comp",teste);

    prof.addSeries("Top_down_Total","Top_down-attr","Top_down-comp");
    prof.addSeries("Bottom_up_Total","Bottom_up-attr","Bottom_up-comp");

    prof.createGroup("Worst CASE - atribuiri","Top_down-attr","Bottom_up-attr");
    prof.createGroup("Worst CASE - comparatii","Top_down-comp","Bottom_up-comp");
    prof.createGroup("Total__Worst","Top_down_Total","Bottom_up_Total");
}

void perf(){
    Average_Case();
    prof.reset("Worst");
    Worst_Case();
    prof.showReport();
}

int main()
{
    demo();
    //perf();

    return 0;
}