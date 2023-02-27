#include <stdio.h>
#include "Profiler.h"
/*
 * * * Ciochina Catalina-Andreea
 * * * * Grupa 30228- semigrupa 1
 *
 *  Am implementat 3 algoritmi de sortare : BubbleSort, InsertionSort si SelectionSort
 *
 * --------ANALIZA---------
 *
 * 1. Average Case == vector nesortat si acelasi vector la toate sortarile
 *  Cel mai optim este Selection Sort, apoi sunt Insertion Sort si BubbleSort.
 *  La COMPARATII, cele mai putine comparatii le face InsertionSort, apoi SelectionSort si BubbleSort
 *  La ATRIBUIRI, cele mai putine atribuiri le face SelectionSort O(2n), apoi BubbleSort si InsertionSort O(n^2)
 *
 * 2. Best Case == vector sortat
 *  Cel mai optim este BubbleSort, cu complexitate liniara de O(n), urmat de InsertionSort- O(2n),
 *  iar apoi de SelectionSort cu O(n^2).
 *  La COMPARATII, cele mai putine le face tot BubbleSort si InsertionSort, in O(n),
 *  iar apoi SelectionSort cu O(n^2).
 *  LA ATRIBUIRI, BubbleSort si SelectionSort fac 0 atribuiri, in O(1), iar InsertionSort, in O(n)
 *
 *
 * 3. Worst Case ==
 *  Cel mai optim algoritm este SelectionSort, apoi sunt InsertionSort si BubbleSort, toti sunt
 *  cu complexitate patratica O(n^2).
 *  La COMPARATII, cele mai putine comparatii le face InsertionSort, cu O(2n), apoi BubbleSort si SelectionSort O(n^2)
 *  La ATIBUIRI, SelectionSort face cele mai putine atribuiri O(n), apoi BubbleSort si InsertionSort O(n^2)
 *
 * BubbleSort    -> alg. stabil (pastreaza ordinea în care apar două elemente egale), memorie O(1)
 * InsertionSort -> alg. stabil
 * SelectionSort -> alg. stabil
 *
 */
#define max_size 10000
#define step_size 100
#define teste 5

Profiler p("sortari");
///numaram operatii pe flag-ri (vect), nu pe indecsi -- de la seminar

void schimba_elem( int *elem1, int *elem2){
    int aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

void bubble_sort(int *a, int n){

    // duce elementul maxim din partea nesortata in partea sortata/ la sfarsitul sirului

    Operation opComp = p.createOperation("bubbleSort-comp",n);
    Operation opAttr = p.createOperation("bubbleSort-attr",n);

    int ok;
    do {

        //la best case --- sir ordonat intra numai o data in for O(n)
        ok=1; //presupunem sirul sortat

        //duce elementul maxim in partea sortata a vectorului = la sfarsit
        for (int i = 0; i < n - 1; i++) {

            opComp.count();
            if(*(a+i) > *(a+i+1)) {

                opAttr.count(3);
                schimba_elem(a+i,a+i+1);
                ok=0;
            }
        }
        //eliminam partea sirului deja sortata
        n--;

    } while (ok==0);


/**
 * aici am facut initial varianta de Bubble Sort in care nu era complexitate liniara in BEST case( care ni se cere).
 *
    for(int i=0; i<n-1; i++){
        /// de ultimul pas i=n-1 nu mai este nevoie pentru ca sirul e deja sortat cand ajunge acolo

        for(int j=0; j<n-1-i; j++){  ///aici n-1-i merge numai pana la elementele sortate

            opComp.count();
            if(*(a+j) > *(a+j+1)){

                opAttr.count(3);
                schimba_elem(a+j,a+j+1);
            }
        }
    }
    */

}

void selection_sort(int *a, int n){
    Operation opComp = p.createOperation("selectionSort-comp",n);
    Operation opAttr = p.createOperation("selectionSort-attr",n);

    int index_minim;

    for(int i=0; i<n-1; i++){

     index_minim=i;
        //cautam indexul minimului din partea nesortata

       for(int j=i+1; j<n; j++) {

          opComp.count();
          if(*(a+j)<*(a+index_minim))

                index_minim = j;

       }
   // in cazul in care a gasit minim in partea nesortata, face interschimb cu elementul de pe poz i
       if( i != index_minim){

           opAttr.count(3);
           schimba_elem(a+i,a+index_minim);
       }
    }

}
void insertion_sort(int *a, int n){

    Operation opComp = p.createOperation("insertionSort-comp",n);
    Operation opAttr = p.createOperation("insertionSort-attr",n);

    int value, index;

    for (int i = 1; i < n; i++) {

        opAttr.count();
        value = *(a+i);

        index = i-1;

        opComp.count();
        if (*(a+index) > value) {

            //duc elementul la pozitia la care trebuie sa fie
            opComp.count();
            while ((index >= 0) && (*(a+index) > value)) {

                opAttr.count(3);
                schimba_elem(a+index+1,a+index);
                index--;
            }
        }

    }
}
void same_vector(int *initial,int *final, int n){

    for(int i=0; i<n; i++){

        *(final+i)=*(initial+i);
    }
}
void printeaza_vector(int *a, int n){

    printf("vectorul este : { ");

    for (int i = 0; i < n; i++) {

          printf("%d ",*(a+i));
    }

    printf(" }\n");
}
void demo() {

    int a[] = {5,77,1,2,19,3,7,1,25,165,165,4,9,256};
    int size = sizeof(a) / sizeof(int);

    int b[size],c[size];
    ///facem copii pentru ceilalti 2 vectori
    same_vector(a,b,size);
    same_vector(a,c,size);

    printf("\nINITIAL\n");
    printeaza_vector(a,size);
    printf("\nBUBBLE_SORT\n");
    bubble_sort(a, size);
    printeaza_vector(a,size);
    printf("\nSELECTION_SORT\n");
    selection_sort(b,size);
    printeaza_vector(b,size);
    printf("\nINSERTION_SORT\n");
    insertion_sort(c,size);
    printeaza_vector(c,size);

}
void Average_Case(){

    int a[max_size],b[max_size],c[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {
            FillRandomArray(a, n,10, 50000, false, UNSORTED);
            same_vector(a,b,n);
            same_vector(a,c,n);

            bubble_sort(a,n);
            selection_sort(b,n);
            insertion_sort(c, n);
        }
    }

    p.divideValues("bubbleSort-attr",teste);
    p.divideValues("insertionSort-attr",teste);
    p.divideValues("selectionSort-attr",teste);


    p.divideValues("insertionSort-comp",teste);
    p.divideValues("bubbleSort-comp",teste);
    p.divideValues("selectionSort-comp",teste);

    p.addSeries("bubbleTotal","bubbleSort-attr","bubbleSort-comp");
    p.addSeries("insertionTotal","insertionSort-attr","insertionSort-comp");
    p.addSeries("selectionTotal","selectionSort-attr","selectionSort-comp");

p.createGroup("Average CASE - atribuiri","bubbleSort-attr","insertionSort-attr","selectionSort-attr");
p.createGroup("Average CASE - comparatii","bubbleSort-comp","insertionSort-comp","selectionSort-comp");
p.createGroup("Total__Average","bubbleTotal","insertionTotal","selectionTotal");

}
void Best_Case(){

    int a[max_size],b[max_size],c[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {
            FillRandomArray(a, n,10, 50000, false, ASCENDING);
            same_vector(a,b,n);
            same_vector(a,c,n);

            bubble_sort(a,n);
            selection_sort(b,n);
            insertion_sort(c, n);
        }
    }

    p.divideValues("bubbleSort-attr",teste);
    p.divideValues("insertionSort-attr",teste);
    p.divideValues("selectionSort-attr",teste);


    p.divideValues("insertionSort-comp",teste);
    p.divideValues("bubbleSort-comp",teste);
    p.divideValues("selectionSort-comp",teste);

    p.addSeries("bubbleTotal","bubbleSort-attr","bubbleSort-comp");
    p.addSeries("insertionTotal","insertionSort-attr","insertionSort-comp");
    p.addSeries("selectionTotal","selectionSort-attr","selectionSort-comp");

    p.createGroup("Best CASE - atribuiri","bubbleSort-attr","insertionSort-attr","selectionSort-attr");
    p.createGroup("Best CASE - comparatii","bubbleSort-comp","insertionSort-comp","selectionSort-comp");
    p.createGroup("Total__Best","bubbleTotal","insertionTotal","selectionTotal");
}

void Worst_Case(){

    int a[max_size],b[max_size],c[max_size];

    for(int n = step_size; n <= max_size; n+=step_size) {
        for (int t = 0; t < teste; t++) {
            FillRandomArray(a, n,10, 50000, false, DESCENDING);
            FillRandomArray(b,n,10,50000,false,ASCENDING);
            b[n-1]=0;
            same_vector(a,c,n);

            bubble_sort(a,n);
            selection_sort(b,n);
            insertion_sort(c, n);
        }
    }

    p.divideValues("bubbleSort-attr",teste);
    p.divideValues("insertionSort-attr",teste);
    p.divideValues("selectionSort-attr",teste);


    p.divideValues("insertionSort-comp",teste);
    p.divideValues("bubbleSort-comp",teste);
    p.divideValues("selectionSort-comp",teste);

    p.addSeries("bubbleTotal","bubbleSort-attr","bubbleSort-comp");
    p.addSeries("insertionTotal","insertionSort-attr","insertionSort-comp");
    p.addSeries("selectionTotal","selectionSort-attr","selectionSort-comp");

    p.createGroup("Worst CASE - atribuiri","bubbleSort-attr","insertionSort-attr","selectionSort-attr");
    p.createGroup("Worst CASE - comparatii","bubbleSort-comp","insertionSort-comp","selectionSort-comp");
    p.createGroup("Total__Worst","bubbleTotal","insertionTotal","selectionTotal");
}
void perf(){

    Average_Case();
    p.reset("BEST");
    Best_Case();
    p.reset("WORST");
    Worst_Case();
    p.showReport();
}
int main() {
   demo();
   //perf();

    return 0;
}

