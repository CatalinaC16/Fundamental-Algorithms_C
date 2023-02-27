#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#define max_size 10000
#define step_size 100
#define teste 5
/**
 * * * Ciochina Catalina-Andreea
 * * * * Grupa 30228- semigrupa 1
 *
 *   * --------ANALIZA--------- * *
 *
 *     Am implementat heapsort, alg care nu e stabil, cu ajutorul metodei de construire bottom-up, extragand maximul si schimband
 *  elem i cu elem de pe size-1-i si apoi aplic heapify, ca sa pastrez conditia de max heap. La varianta de construire bottom-up,
 *  pornim de la un heap, reprezentat ca un array. Pornim cu parcurgerea de la "nodurile" care nu sunt frunze(n/2)-1 ca
 * (de la 0 cu parcurgerea) si ne uitam in jos, la copii sai si in cazul in care nu respecta conditia de max heap,
 *  facem heapify( heapify the subtree). Complexitate O(n*logn).
 *
 *    La quicksort, alg. care este stabil, am ales ca pivot ultimul element si  elementele mai mici decat pivot se plaseaza in stanga,
 * iar cele mai mari in dreapta pivotului, j<= pivot, inc i si swap la elem de pe poz i si j, proces repetat pana cand j ajunge
 * la pozitia pivotului, in final pivotul se pune pe pozitia i+1 (partitionare). Dupa fiecare partitie, apelam recursiv quicksort pe
 * cele 2 subsiruri obtinute la fiecare pas, repetand intregul procedeu pana cand start >= end. Worst Case O(n^2)
 * -> sir sortat. Best si Avg O(n*logn).
 *
 *    La quickselect randomized alegem pivotul random, schimbam elem de pe poz random cu ultimul si aplicam partition,
 * exact ca la quicksort, diferenta fiind in faptul ca daca elementul cautat este in subsirul din stg/dr( mergem numai pe
 * subsirul respectiv)sau este chiar pivotul.
 *
 *   Ambele sortari se fac in-place, quicksort este mai eficient, insa ambii au complexitatea O(n*logn).
 *
 */

Profiler prof("HeapSort&QuickSort");

int random(int min, int max) {

       return ( rand() % (max - min + 1) + min);
}

void schimba_elem(int *elem1, int *elem2) {
    int aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

void acelasi_vector(int *initial, int *final, int n) {
    for (int i = 0; i < n; i++) {
        *(final + i) = *(initial + i);
    }
}

void printeaza_vector(int *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", *(a + i));
    }
    printf("\n");
}

/// parcurgem de la 0 la sizeA-1 deci copii lui a[i] sunt la pozitiile 2*i+1 si 2*i+2

int copil_stanga(int i) {
    return 2 * i + 1;
}

int copil_drepta(int i) {
    return 2 * i + 2;
}

/// heapify (recursiv) utilizat la metoda de construire bottom-up a heap-ului

void heapify(int *a, int n, int i, int *opComp, int *opAttr) {

    int curent = i;
    int stg = copil_stanga(i);
    int dr = copil_drepta(i);

    if (stg < n) {

        *opComp += 1;

        if (*(a + stg) > *(a + curent))
            curent = stg;
    }
    if (dr < n) {

        *opComp += 1;

        if (*(a + dr) > *(a + curent)) {
            curent = dr;
        }
    }
    ///inseamna ca nu este la locul bun si trebuie sa parcurgem de acolo in jos => heapify the subtree
    ///ca sa ii stabilim locul unde trebuie sa fie
    if (curent != i) {

        *opAttr += 3;
        schimba_elem(a + i, a + curent);
        heapify(a, n, curent, opComp, opAttr);
    }
}

void heap_bottom_up(int *a, int n, int *comp, int *attr) {

    int co = 0, atr = 0;
    ///eliminam frunzele - nodurile care nu au copil drept si stang => (n/2)-1
    /// si facem heapify la fiecare element ramas

    for (int i = ((n / 2) - 1); i >= 0; i--) {

        heapify(a, n, i, &co, &atr);
        *comp += co;
        *attr += atr;
        co = 0;
        atr = 0;
    }

}

/// *** HEAPSORT *** //  bottom_up_heapsort
void heap_sort(int *a, int n) {

    Operation opComp = prof.createOperation("HeapSort-comp", n);
    Operation opAttr = prof.createOperation("HeapSort-attr", n);

    int op = 0, atr = 0;
    ///construim heap-ul bottom-up
    heap_bottom_up(a, n,&op,&atr);
    ///sortam
    for (int i = n - 1; i >= 0; i--) {
        opAttr.count(3);
        schimba_elem(a, a + i);
        heapify(a, i, 0, &op, &atr);
        opAttr.count(atr);
        opComp.count(op);
        op = 0;
        atr = 0;
    }

}

/// *** QUICKSORT *** ///
int partition(int *a, int start, int end, int *co, int *atr) {
    *atr += 1;
    int pivot = a[end];

    int i = start-1;

    for (int j = start; j<= end - 1; j++) {

        *co += 1;

            if (*(a + j) < pivot) {
                i++;
                *atr += 3;
                schimba_elem(a + j, a + i);
            }
    }
    *atr += 3;
    schimba_elem(a + end, a + i+1);

    i+=1;
    return i;
}

void quick(int *a, int start, int end, int* com, int* att) {
   int ca=0,ao=0;
    if (start >= end)
        return;
    else if (start < end) {
        int piv_index = partition(a, start, end, &ca, &ao);
        *com += ca;
        *att += ao;
        ca=0; ao=0;
        quick(a, start, piv_index - 1,&ca,&ao);
        quick(a, piv_index + 1, end,&ca,&ao);
    }
    *com += ca;
    *att += ao;

}
void quicksort(int *a,int size, int start, int end){
    int comparatii=0,atribuiri=0;

    Operation opComp = prof.createOperation("QuickSort-comp", size);
    Operation opAttr = prof.createOperation("QuickSort-attr", size);

    quick(a,start,end, &comparatii,&atribuiri);

    opComp.count(comparatii);
    opAttr.count(atribuiri);

}


/// *** QUICKSELECT Randomized *** ///
int partitionRand(int *a, int start, int end, int *co, int *atr) {

    int x=0,y=0;

    int random_index= (rand()%(end-start+1))+start;
    schimba_elem(a+random_index,a+end);
    partition(a,start,end,&x , &y);

}
int quickSelect (int *a, int start, int end, int p){
    int x=0,y=0;

    if(start==end)
        return *(a+start);

    int q = partitionRand(a,start,end,&x,&y );
    int pivot = q-start+1;
    if(p == pivot)
        return *(a+q);
    else if(p < pivot)
        return quickSelect(a,start,q-1,p);
    else
        return quickSelect(a,q+1,end,p-pivot);
}


/// *** InsertionSort  ITERATIV + RECURSIV *** ///
void insertion_sort(int *a, int n){

    Operation opComp = prof.createOperation("insertionSort-comp",n);
    Operation opAttr = prof.createOperation("insertionSort-attr",n);

    int value, index;

    for (int i = 1; i < n; i++) {

        opAttr.count();
        value = *(a+i);

        index = i-1;

        opComp.count();
        if (*(a+index) > value) {

            /// duc elementul la pozitia la care trebuie sa fie
            opComp.count();
            while ((index >= 0) && (*(a+index) > value)) {

                opComp.count();
                opAttr.count(3);
                schimba_elem(a+index+1,a+index);
                index--;
            }
        }

    }
}

void insertion_sortR(int *a, int n, int *Comp, int *Attr)
{ int co=0, atr=0;

    if (n <= 1)
        return;

    insertion_sortR( a, n-1 ,&co,&atr);

    *Comp += co;
    *Attr += atr;

    *Attr += 1;
    int last = *(a+n-1);
    int j = n-2;

    *Comp += 1;
    while (j >= 0 && *(a+j) > last)
    {
        *Comp += 1;
        *Attr += 3;
        *(a+j+1) = *(a+j);
        j--;
    }
    *Comp += 1;
    *(a+j+1) = last;
}
void insertion_sortR(int *a, int n){
    int Comp=0, Attr=0;

    Operation opComp = prof.createOperation("insertionSortR-comp",n);
    Operation opAttr = prof.createOperation("insertionSortR-attr",n);

    insertion_sortR(a, n, &Comp, &Attr);

    opComp.count(Comp);
    opAttr.count(Attr);
}

void demo() {
    int quick[] = {10, 25, 15, 10, 9, 10, 13, 27};
    int size = sizeof(quick) / sizeof(int);
    int heap[size], insert1[size], insert2[size], quickRand[size];

    acelasi_vector(quick, heap, size);
    acelasi_vector(quick, insert1, size);
    acelasi_vector(quick, insert2, size);
    acelasi_vector(quick, quickRand, size);

    printf("INITIAL: ");
    printeaza_vector(quick, size);

    printf("QUICKSORT: ");
    quicksort(quick,size,0,size-1);
    printeaza_vector(quick,size);

    printf("HEAP_SORT: ");
    //heap_bottom_up(sort,size);
    heap_sort(heap, size);
    printeaza_vector(heap, size);

    printf("INSERTION_SORT ITERATIV: ");
    insertion_sort(insert1,size);
    printeaza_vector(insert1,size);

    printf("INSERTION_SORT RECURSIV: ");
    insertion_sortR(insert2,size);
    printeaza_vector(insert2,size);

    printf("QUICKSELECT Randomized: ");
    int x = 6;
    srand(time(0));
    printf("Al %d cel mai mic numar este %d",x,quickSelect(quickRand,0,size-1,x));
}

void Average_Case(){
    int a[max_size],b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, UNSORTED);
            acelasi_vector(a,b,n);
            heap_sort(a,n);
            quicksort(b,n,0,n-1);
        }
    }
    prof.divideValues("HeapSort-attr",teste);
    prof.divideValues("QuickSort-attr",teste);

    prof.divideValues("HeapSort-comp",teste);
    prof.divideValues("QuickSort-comp",teste);

    prof.addSeries("HeapSort_Total","HeapSort-attr","HeapSort-comp");
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");

    prof.createGroup("Average CASE - atribuiri","HeapSort-attr","QuickSort-attr");
    prof.createGroup("Average CASE - comparatii","HeapSort-comp","QuickSort-comp");
    prof.createGroup("Total__Average","HeapSort_Total","QuickSort_Total");
}

void Worst_Case(){
    int a[max_size],b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, ASCENDING);
            FillRandomArray(b, n,10, 50000, false, DESCENDING);

            heap_sort(a,n);
            quicksort(b,n,0,n-1);
        }
    }
    prof.divideValues("HeapSort-attr",teste);
    prof.divideValues("QuickSort-attr",teste);

    prof.divideValues("HeapSort-comp",teste);
    prof.divideValues("QuickSort-comp",teste);

    prof.addSeries("HeapSort_Total","HeapSort-attr","HeapSort-comp");
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");

   // prof.createGroup("Worst CASE - atribuiri","HeapSort-attr","QuickSort-attr");
   // prof.createGroup("Worst CASE - comparatii","HeapSort-comp","QuickSort-comp");
   // prof.createGroup("Total__Worst","HeapSort_Total","QuickSort_Total");
}
void Best_Case() {
    int a[max_size], b[max_size];

    for (int n = step_size; n <= max_size; n += step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(b, n, 10, 50000, true, DESCENDING);

            *(a + n) = random(100, 50000);
            for (int i = 0; i < n; i++) {
                if (i % 2 == 0)
                     *(a + i) = random(10, *(a + n));
                else
                 *(a + i) = random(*(a + n), 50000);

            }

            heap_sort(b, n);
            quicksort(a, n, 0, n - 1);
        }
    }
    prof.divideValues("HeapSort-attr",teste);
    prof.divideValues("QuickSort-attr",teste);

    prof.divideValues("HeapSort-comp",teste);
    prof.divideValues("QuickSort-comp",teste);

    prof.addSeries("HeapSort_Total","HeapSort-attr","HeapSort-comp");
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");

    prof.createGroup("Best CASE - atribuiri","HeapSort-attr","QuickSort-attr");
    prof.createGroup("Best CASE - comparatii","HeapSort-comp","QuickSort-comp");
    prof.createGroup("Total__Best","HeapSort_Total","QuickSort_Total");
}
void Best_insertion(){
    int a[max_size],b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, ASCENDING);
            acelasi_vector(a,b,n);
            insertion_sortR(a,n);
            insertion_sort(b,n);
        }
    }
    prof.divideValues("insertionSortR-attr",teste);
    prof.divideValues("insertionSort-attr",teste);

    prof.divideValues("insertionSortR-comp",teste);
    prof.divideValues("insertionSort-comp",teste);

    prof.addSeries("insertionSortR_Total","insertionSortR-attr","insertionSortR-comp");
    prof.addSeries("insertionSort_Total","insertionSort-attr","insertionSort-comp");

    prof.createGroup("Best CASE - atribuiri","insertionSortR-attr","insertionSort-attr");
    prof.createGroup("Best CASE - comparatii","insertionSortR-comp","insertionSort-comp");
    prof.createGroup("Total__Best","insertionSortR_Total","insertionSort_Total");
}
void Avg_insertion_TIMP(){
    int a[max_size],b[max_size],c[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        FillRandomArray(a, n,10, 50000, false, UNSORTED);
        acelasi_vector(a,b,n);
        acelasi_vector(a,c,n);

       prof.startTimer("insertion_sortRec--Timp",n);
       for (int t = 0; t < teste; t++) {

            insertion_sortR(a,n);
            acelasi_vector(c,a,n);
       }
       prof.stopTimer("insertion_sortRec--Timp",n);

        prof.startTimer("insertion_sort--Timp",n);
        for (int t = 0; t < teste; t++) {

            insertion_sort(b,n);
            acelasi_vector(c,b,n);
        }
        prof.stopTimer("insertion_sort--Timp",n);

    }

}
void Avg_insertion(){

    int a[max_size],b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
           for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, UNSORTED);
            acelasi_vector(a,b,n);
            insertion_sortR(a,n);
            insertion_sort(b,n);
        }

    }
    prof.divideValues("insertionSortR-attr",teste);
    prof.divideValues("insertionSort-attr",teste);

    prof.divideValues("insertionSortR-comp",teste);
    prof.divideValues("insertionSort-comp",teste);

    prof.addSeries("insertionSortR_Total","insertionSortR-attr","insertionSortR-comp");
    prof.addSeries("insertionSort_Total","insertionSort-attr","insertionSort-comp");

    prof.createGroup("Average CASE - atribuiri","insertionSortR-attr","insertionSort-attr");
    prof.createGroup("Average CASE - comparatii","insertionSortR-comp","insertionSort-comp");
    prof.createGroup("Total__Average","insertionSortR_Total","insertionSort_Total");
}
void Worst_insertion(){
    int a[max_size],b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, DESCENDING);
            acelasi_vector(a,b,n);
            insertion_sortR(a,n);
            insertion_sort(b,n);
        }
    }
    prof.divideValues("insertionSortR-attr",teste);
    prof.divideValues("insertionSort-attr",teste);

    prof.divideValues("insertionSortR-comp",teste);
    prof.divideValues("insertionSort-comp",teste);

    prof.addSeries("insertionSortR_Total","insertionSortR-attr","insertionSortR-comp");
    prof.addSeries("insertionSort_Total","insertionSort-attr","insertionSort-comp");

    prof.createGroup("Worst CASE - atribuiri","insertionSortR-attr","insertionSort-attr");
    prof.createGroup("Worst CASE - comparatii","insertionSortR-comp","insertionSort-comp");
    prof.createGroup("Total__Worst","insertionSortR_Total","insertionSort_Total");
}
void Average_Quick(){
    int a[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(a, n,10, 50000, false, UNSORTED);
            quicksort(a,n,0,n-1);
        }
    }
    prof.divideValues("QuickSort-attr",teste);
    prof.divideValues("QuickSort-comp",teste);
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");
}

void Worst_Quick(){
    int b[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {

            FillRandomArray(b, n,10, 50000, false, DESCENDING);
            quicksort(b,n,0,n-1);
        }
    }
    prof.divideValues("QuickSort-attr",teste);
    prof.divideValues("QuickSort-comp",teste);
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");
}

void Best_Quick(){
    int a[max_size];

    for (int n = step_size; n <= max_size; n += step_size) {
        for (int t = 0; t < teste; t++) {
            *(a + n) = random(100, 50000);
            for (int i = 0; i < n; i++) {
                if (i % 2 == 0)
                    *(a + i) = random(10, *(a + n));
                else
                    *(a + i) = random(*(a + n), 50000);
            }

            quicksort(a, n, 0, n - 1);
        }
    }

    prof.divideValues("QuickSort-attr",teste);
    prof.divideValues("QuickSort-comp",teste);
    prof.addSeries("QuickSort_Total","QuickSort-attr","QuickSort-comp");
}

void perf() {

    prof.reset("Average Quick");
    Average_Quick();
    prof.reset("Worst Quick");
    Worst_Quick();
    srand(time(0));
    prof.reset("Best Quick");
    Best_Quick();

    prof.reset("Average Comparatie");
    Average_Case();
    prof.reset("Worst Comparatie");
    Worst_Case();
    srand(time(0));
    prof.reset("Best Comparatie");
    Best_Case();


    prof.reset("Average Case Insertion TIME");
    Avg_insertion_TIMP();

    prof.reset("Average Case Insertion");
    Avg_insertion();

    prof.reset("Best Case Insertion");
    Best_insertion();
    prof.reset("Worst Case Insertion");
    Worst_insertion();

    prof.showReport();
}

int main() {

  demo();
   //perf();

    return 0;
}
