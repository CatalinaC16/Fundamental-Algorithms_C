#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"
/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *  Am implementat algoritmul de interclasare a k liste ordonate, cu un numar total de n elemente,
 *  * pentru demo -> k liste ordonate de dimensiune diferita, obtinuta random, rand%(nr_elem_ramase dupa inserarea
 *  random in fiecare lista +1), insa intotdeauna > 1 element la fiecare.
 *  * pentru perf -> k liste ordonate, fiecare de dimensiune fixa n/k
 *  Am definit 3 structuri, 1 structura de node(valoare, pointer la next elem), 1 structura de lista(care are in first
 *  un pointer spre primul node din lista) si 1 structura de vector( valoare, nr lista provenienta);
 *  La interclasare am definit o lista de liste si un vector de tipul structurii (val, nr_lista_provenienta)
 *  si o lista simpla finala de n elemente, in care se va afla la final interclsarea celor k liste/ n elemente sortate cresc.
 *  Popularea listelor, explicata anterior, am pregatit in vector elementele de pe prima pozitie din fiecare lista
 *  alaturi de nr_listei_de_provenienta. Am construit pe vectorul in discutie MinHeap-ul.
 *  Am parcurs de la 0,n -> la fiecare pas am aplicat heapify pe vectorul de MinHeap, am inserat valoarea din root
 *  in lista finala(insert last am aplicat la liste), apoi ca sa adaug un nou element am verificat daca lista
 *  din care s-a extras nodul nu a ajuns la epuizarea nodurilor(lista vida), in acest caz am adaugat valoarea 501,
 *  care nu putea sa intre in lista finala deoarece sunt n parcurgeri = n elem, iar range-ul maxim pe care l-am
 *  stabilit la generarea random de elemente este 500, in cazul in care lista nu a devenit vida, am exstras in vector,
 *  tot in root, urmatoarea valoare din lista de provenienta a anteriorului element.
 *
 *  La grafice, pentru cazul in care am mentinut constanta valoarea lui k= pe 5/10/100 si am variat valoarea lui n cu
 *  increment de 100, n=100,10000 complexitatea este O(nlogk)~= O(n)
 *  Pentru cazul in care am mentinut constanta valoarea lui n=10000 si am variat valoarea lui k intre 10 si 500 cu
 *  increment 10, complexitatea este O(10000logk)~= O(logk)
 *
 **/
Profiler prof("MergeKLists");

typedef struct Node{
    int data;
    struct Node *next;
} Node;

typedef struct Lista{
    Node* first;
}Lista;

typedef struct VectorHeap{
    int data;
    int nrlista;
} Vector;

void schimba_elem(int *elem1, int *elem2){
    int aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

/****************/
/*** MIN_HEAP ***/
/****************/

/// parcurgem de la 0 la sizeA-1 deci copii lui a[i] sunt la pozitiile 2*i+1 si 2*i+2
int copil_stanga(int i){
    return 2 * i + 1;
}

int copil_drepta(int i){
    return 2 * i + 2;
}
/// heapify (recursiv) utilizat la metoda de construire bottom-up a MinHeap-ului

void heapify(Vector **a, int size, int i, int *opComp, int *opAttr){
    int curent = i;
    int stg = copil_stanga(i);
    int dr = copil_drepta(i);

    if (stg < size)
    {

        *opComp += 1;

        if (a[stg]->data < a[curent]->data)
            curent = stg;
    }
    if (dr < size)
    {

        *opComp += 1;

        if (a[dr]->data < a[curent]->data)
        {
            curent = dr;
        }
    }
    ///inseamna ca nu este la locul bun si trebuie sa parcurgem de acolo in jos => heapify the subtree
    ///ca sa ii stabilim locul unde trebuie sa fie
    if (curent != i)
    {

        *opAttr += 3;
        schimba_elem(&a[i]->data, &a[curent]->data);
        schimba_elem(&a[i]->nrlista, &a[curent]->nrlista);
        heapify(a, size, curent, opComp, opAttr);
    }
}

void heap_bottom_up(Vector **a, int k, int *comp, int *attr){
    int co = 0, atr = 0;
    ///eliminam frunzele - nodurile care nu au copil drept si stang => (n/2)-1
    /// si facem heapify la fiecare element ramas

    for (int i = ((k / 2) - 1); i >= 0; --i)
    {

        heapify(a, k, i, &co, &atr);
        *comp += co;
        *attr += atr;
        co = 0;
        atr = 0;
    }
}
/*****************************/
/***LISTA SIMPLU INLANTUITA***/
/*****************************/

Lista* creaza_lista(){
    Lista *list = (Lista *)malloc( sizeof(Lista));
    list->first = NULL;
    return list;
}

Node *creaza_node(int valoare){
    Node *node = (Node *) malloc(sizeof(Node));
    node->data = valoare;
    node->next = NULL;
    return node;
}

void inserare_sfarsit_lista(Lista* lista, int valoare){
    if(lista ==  NULL)
        return;
    ///construim nodul
    Node *node = creaza_node(valoare);

    if (lista->first == NULL)
    {
        lista->first = node;
        lista->first->next = NULL;
        return;
    }
    else
    {
        ///asignam capatul listei
        Node *current = lista->first;

        while (current->next != NULL)
        {
            ///parcurgem lista
            current = current->next;
        }
        ///punem valoarea la sfarsit
        current->next = node;
        current->next->next = NULL;
    }

}

void afiseaza_lista(Lista* lista){
    ///asignam capatul listei
    Node *current = lista->first;
    ///parcurgem lista
    while (current->next != NULL)
    {
        printf("%d->", current->data);
        current = current->next;
    }
    printf("%d", current->data);
    printf("\n");
}

int extrage_primul_element(Lista* lista){
    ///primul element
    Node *node = lista->first;
    int data;
    ///schimbam first ul sa pointeze spre urmatorul element
    if(lista->first == NULL)
        return 501;
    else {
        lista->first = node->next;
        data = node->data;
        /// eliberam memoria nodului sters
        node->next=NULL;
        free(node);
    }
    return data;
}

void populare_liste_random(Lista** liste, int k, int n){
    srand(time(0));
    int random1[n],random2[n];
    int nr_elemente_ramase = (n-k);

    FillRandomArray(random1, k, 0, 150, false, ASCENDING);
    for (int i = 0; i < k; ++i)
    {
        ///liste goale
        liste[i]= creaza_lista();
        ///lista cu un element
        inserare_sfarsit_lista(*(liste+i),*(random1+i));
    }

    for (int i = 0; i < k; ++i)
    {
        int lung_lista = 0;

        if(nr_elemente_ramase > 0){

           lung_lista = (rand() % (nr_elemente_ramase + 1));
           ///aici am incercat sa fac cumva sa nu puna deodata un numar mare de elemente intr-o lista
           ///si dupa celalalte liste sa ramana cu un nr mic de elem
           if(lung_lista > (nr_elemente_ramase/2)){
               lung_lista= lung_lista % 2;
           }

           FillRandomArray(random2, lung_lista, 150, 350, false, ASCENDING);

            for (int j = 0; j < lung_lista; ++j)
            {
                inserare_sfarsit_lista(*(liste+i), *(random2 + j));
            }

            nr_elemente_ramase= nr_elemente_ramase - lung_lista;
        }
    }
    ///nu am epuizat toate elementele
    int x[nr_elemente_ramase];
    if(nr_elemente_ramase>0)
    {
        FillRandomArray(x, nr_elemente_ramase+1, 350, 500, false, ASCENDING);
        for (int i = 0; i < nr_elemente_ramase; ++i)
        {
            inserare_sfarsit_lista(*(liste+k-1),*(x+i));
        }
    }
}

void populare_liste(Lista** liste, int k, int n){
    int random[n/k];
    for (int i = 0; i < k; ++i)
    {
        ///liste goale
        liste[i]= creaza_lista();

    }
    for (int i = 0; i < k; ++i)
    {
        FillRandomArray(random, n/k, 0, 500, false, ASCENDING);

        for (int j = 0; j < n/k; ++j)
        {
            /// umplem fiecare lista cu n/k elemente
            inserare_sfarsit_lista(*(liste+i), *(random + j));
        }
    }
}
void populare_vectorMinHeap(Vector **vector, int size, Lista **liste, int *atr){
    ///populam vectorul ptr construire MinHeap, primul element din fiecare lista
    for (int i = 0; i < size; ++i)
    {
        *atr+=1;
        vector[i]->data = extrage_primul_element(*(liste+i));
        vector[i]->nrlista = i;
    }
}

Vector **creaza_vector(int size){
    ///alocam memorie vectorului
    Vector **v = (Vector **) malloc(size * sizeof(Vector *));
    for (int i = 0; i < size; ++i)
    {
        v[i] = (Vector *) malloc(sizeof(Vector));
    }
    return v;
}
void afisare_liste(Lista** liste, int k){
    for (int i = 0; i < k; ++i)
    {
        Lista* lista_i = *(liste + i);
        afiseaza_lista(lista_i);
    }
    printf("\n");
}

///***  diferentele dintre cele 2 functii este popularea listelor( prima- generare random dimensiune,
///***  a doua generare cu dimensiune fixa n/k
///***  si ca pe cea random o apelez in demo si pe cealalta
void interclasare_liste_random_dimensiune(int k, int n,char* nume)
{
    int op=0;
    Operation operatii = prof.createOperation(nume, n);

    Vector **vector = creaza_vector(k);
    Lista* liste[k];

    populare_liste_random(liste,k,n);
    afisare_liste(liste,k);

    ///adaugam primul element din fiecare lista care mai are elemente
    populare_vectorMinHeap(vector, k, liste,&op);

    ///construim MinHeap -> in root/pe poz 0 va fi minimul celor k elemente
    heap_bottom_up(vector, k, &op, &op);

    Lista *final = creaza_lista(); ///initializam lista finala in care vor fi interclasate listele
    for (int i = 0; i < n; ++i)
    {
        heapify(vector, k, 0, &op, &op);
        ///pe pozitia 0(radacina) avem minimul in vectorul reprezentat ca MinHeap
        inserare_sfarsit_lista(final, vector[0]->data);

        ///daca am terminat lista din care provenea elemntul din radacina MinHeap-ului
        ///trebuie sa o ignoram
        if (liste[vector[0]->nrlista] == NULL)
        { op+=1;
            ///punem mai mult decat maxim,ca sa refaca la heapify proprietatea de heap,
            ///am pus range ul maxim 500, nu ia valorile de 501 in considerare, pentru ca trebuie sa introduca
            ///numai n elemente in total
            vector[0]->data = 501;
        }
        else
        { op+=1;
            ///lista mai are elemente, daugam urmatorul element din lista din care am obtinut minimul
            vector[0]->data = extrage_primul_element(liste[vector[0]->nrlista]);
        }
    }
    printf("Interclasarea celor k=%d liste ordonate cu %d elemente in total este:\n", k,n);
    afiseaza_lista(final);
    operatii.count(op);
    printf("\n");
}

void interclasare_liste(int k, int n,int n_op,char* nume)
{
    int op=0;
    Operation operatii = prof.createOperation(nume, n_op);

    Vector **vector = creaza_vector(k);
    Lista* liste[k];

    populare_liste(liste, k, n);

    ///adaugam primul element din fiecare lista care mai are elemente
    populare_vectorMinHeap(vector, k, liste,&op);

    ///construim MinHeap -> in root/pe poz 0 va fi minimul celor k elemente
    heap_bottom_up(vector, k, &op, &op);

    Lista *final = creaza_lista(); ///initializam lista finala in care vor fi interclasate listele
    for (int i = 0; i < n; ++i)
    {
        heapify(vector, k, 0, &op, &op);
        ///pe pozitia 0(radacina) avem minimul in vectorul reprezentat ca MinHeap
        inserare_sfarsit_lista(final, vector[0]->data);

        ///daca am terminat lista din care provenea elemntul din radacina MinHeap-ului
        ///trebuie sa o ignoram
        if (liste[vector[0]->nrlista] == NULL)
        { op+=1;
            ///punem mai mult decat maxim,ca sa refaca la heapify proprietatea de heap,
            ///am pus range ul maxim 500, nu ia valorile de 501 in considerare, pentru ca trebuie sa introduca
            ///numai n elemente in total
            vector[0]->data = 501;
        }
        else
        { op+=1;
            ///lista mai are elemente, daugam urmatorul element din lista din care am obtinut minimul
            vector[0]->data = extrage_primul_element(liste[vector[0]->nrlista]);
        }
    }
    printf("Interclasarea celor k=%d liste ordonate cu %d elemente in total este:\n", k,n);
    operatii.count(op);
    printf("\n");

}
void demo()
{
    int n=10,k=4;
    interclasare_liste_random_dimensiune(k,n,"try0");
    printf("**************************************\n");
    n = 15;k = 5;
    interclasare_liste_random_dimensiune(k, n,"try1");
    printf("**************************************\n");
    n=18;k=3;
    interclasare_liste_random_dimensiune(k, n,"try2");
    printf("**************************************\n");
    n=30;k=6;
    interclasare_liste_random_dimensiune(k,n,"try3");
    printf("**************************************\n");
    n=50;k=6;
    interclasare_liste_random_dimensiune(k,n,"try4");
    printf("**************************************\n");
}
void perf(){

    /// 5 liste, de n/5 elemente
    for (int n = 100; n <= 10000; n += 100)
    {
        interclasare_liste(5, n,n,"k1=5");
    }
    ///10 liste, de n/10 elemente
    for (int n = 100; n <= 10000; n += 100)
    {
        interclasare_liste(10, n,n,"k2=10");
    }
    ///100 liste, de n/100 elemente
    for (int n = 100; n <= 10000; n += 100)
    {
        interclasare_liste(100, n,n,"k3=100");
    }
    prof.createGroup("toate", "k1=5", "k2=10", "k3=100");

    for (int k = 10; k <= 500; k += 10)
    {
       interclasare_liste(k, 10000,k, "n=10000");
    }
    prof.showReport();

}
int main()
{
    demo();
    //perf();
    return 0;
}

