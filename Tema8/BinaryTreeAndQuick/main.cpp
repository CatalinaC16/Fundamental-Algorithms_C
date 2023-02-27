#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define STEP_SIZE 100
#define MAX_SIZE 10000
#define NR_TESTS 50

Profiler prof("BinaryTREEandHibridQuick");

/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *    Am implementat algoritmul de creare si parcurgere a unui arbore binar. Parcurgerile le-am facut in inordine.
 *  Pentru crearea arborelui am folosit o structura de tipul Node care contine valoarea, pointer catre copil stanga si
 *  pointer catre copil dreapta. Am adaugat copii ptr.fiecare node (2*i+1,2*i+2).
 *    La Inorder recursiv cat timp nu ajungeam la un node null, mergeam recursiv ptr fiecare pe subarbore stanga, printam inordine rad,
 * apoi mergeam pe subarbore dreapta.
 *    La Inorder iterativ, am folosit teoria de stiva, am creat o structura care sa faca un nodeStiva( de tipul Stiva ) care sa contina
 * node ul curent, de tipul Node si pointer la urmatorul nodeStiva. Am implementat operatiile de push si pop de pe stiva. Si initializam stiva,
 * setam current node la radacina arborelui binar,punem node current pe stiva pana ajungem la null in subarbore stang, apoi scoatem node uri din
 * stiva, printam node ul curent, si fixam node ul curent la copil dreapta si repetam pana cand stiva este din nou nula, caz in care inseamna
 * ca nu mai sunt node uri de introdus.
 *    La quicksort am ales ca pivot ultimul element si  elementele mai mici decat pivot se plaseaza in stanga,
 * iar cele mai mari in dreapta pivotului, j<= pivot, inc i si swap la elem de pe poz i si j, proces repetat pana cand j ajunge
 * la pozitia pivotului, in final pivotul se pune pe pozitia i+1 (partitionare). Dupa fiecare partitie, apelam recursiv quicksort pe
 * cele 2 subsiruri obtinute la fiecare pas, repetand intregul procedeu pana cand start >= end. Worst Case O(n^2)-> sir sortat.
 * Best si Avg O(n*logn).
 *    La quicksort, varianta HIBRIDIZATA, acelasi algoritm ca la quicksort pe siruri de multe elemente, iar pe nr de elemente mai
 * mic fac insertionSort. Quicksort se comporta bine pe size mare al unui sir, iar insertionsort pe size-uri mici.
 *
 *  Parcurgerile sunt in O(n), iar QuickSort Hibridizat este O(n^2).
 *
*/
typedef struct Node
{
    int data;
    Node* stanga;
    Node* dreapta;
} Node;

typedef struct Stiva
{
    Node* curent;
    Stiva* urmator;
};

Node* creareNode(int data)
{   ///alocam nodul
    Node* node = (Node*) calloc(1,sizeof(Node));
    node->data = data;
    /// pointeri catre copii nulli
    node->stanga = NULL;
    node->dreapta = NULL;
    return node;
}

Node* build_BinaryTREE(int *a,int i, int n)
{ ///setam radacina la null
    Node *root = NULL;
    if (i < n)
    {
        ///cream radacina
        root = creareNode(a[i]);
        ///punem pentru fiecare root copii care ar trebui
        root->stanga = build_BinaryTREE(a,2*i+1, n);
        root->dreapta = build_BinaryTREE(a,2*i+2, n);
    }
    return root;
}

void inOrder(Node* root,Operation opps)
{
    if (root != NULL)
    {
        ///subarbore stang
        inOrder(root->stanga,opps);
        opps.count();
        ///inorder
        printf("%d ",root->data);
        ///subarbore dreapta
        inOrder(root->dreapta,opps);
    }
}
void push_Stiva(Stiva** top, Node* node)
{
  ///alocam un node de tipul Stiva
    Stiva* nodeStiva = (Stiva *) calloc(1, sizeof(Stiva));

    ///legam nodul
    nodeStiva->curent = node;
    ///restabilim legatura nodului pe care il inseram
    nodeStiva->urmator = (*top);
    ///restablim capatul stivei
    (*top) = nodeStiva;
}

Node* pop_Stiva(Stiva** top)
{
    ///am initializat 2 noduri, unul de arbore si unul de tipul structurii Stiva
    Node* node;
    Stiva* nodeStiva;

    ///luam nodul
    nodeStiva = *top;
    node = nodeStiva->curent;
    ///restabilim capatul stivei
    *top = nodeStiva->urmator;

    return node;
}
int stivaGoala(Stiva *stiva)
{
    if(stiva == NULL)
        return 1;
    else
        return 0;

}
void inOrderStiva(Node *root, Operation ops)
{
   ///setam current node la radacina
    Node* current = root;
    ///initializam stiva goala
    Stiva* s = NULL;
    ///presupunem ca adaugam ceva in stiva
    int ok = 0;

    ///iese din while cand stiva devine goala
    while (ok!=1)
    {
        if(current !=  NULL)
        {
            ///punem node current pe stiva
            push_Stiva(&s, current);
            ///meregem pe subarbore stang pana ajungem la NULL
            current = current->stanga;
        }
        else
        {
            ///stiva nu e goala
            if (stivaGoala(s) != 1)
            {
                ///scoatem de pe stiva in current
                current = pop_Stiva(&s);
                ///inorder, afisam elem
                ops.count();
                printf("%d ", current->data);
                ///mergem pe subarbore dreapta
                current = current->dreapta;
            }
            else
                ok = 1;///stiva goala,am terminat nodurile
        }
    }
}

void acelasi_vector(int *initial, int *final, int n)
{
    ///functie pentru copiere array
    for (int i = 0; i < n; i++)
    {
        *(final + i) = *(initial + i);
    }
}

void printeaza_vector(int *a, int n)
{
    printf("vectorul este : { ");

    for (int i = 0; i < n; i++)
    {

        printf("%d ",*(a+i));
    }
    printf("}\n");

}

void schimba_elem( int *elem1, int *elem2)
{
    int aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

void insertion_sort(int *a,int start, int n,Operation asign2,Operation comp2) {
    int value, index;

    for (int i = start + 1; i <= n; i++) {
        value = *(a + i);
        index = i - 1;
        comp2.count();
        if (*(a + index) > value) {
        ///duc elementul la pozitia la care trebuie sa fie
            comp2.count();
            while ((index >= start) && (*(a + index) > value)) {
                ///cu elem mai mici inaintea sa
                comp2.count();
                asign2.count(3);
                schimba_elem(a + index + 1, a + index);
                index--;
            }
        }
    }
}

int partition(int *a, int start, int end,Operation asign,Operation comp)
{
    ///setam ca pivot ultimul element
    int pivot = a[end];
    int i = start;

    for (int j = start; j< end; j++)
    {
        comp.count();
        if (*(a + j) < pivot)
        {
            asign.count(3);
            schimba_elem(a + j, a + i);
            i++;
        }
    }
    asign.count(3);
    schimba_elem(a + end, a + i);
    return i;
}

void quickSort(int *a, int start, int end,Operation asign1,Operation comp1)
{
    if (start >= end)
        return;
    else if (start < end)
    {
        ///suntem inca intre limite
        int piv_index = partition(a, start, end,asign1,comp1);
        quickSort(a, start, piv_index - 1,asign1,comp1);
        quickSort(a, piv_index + 1, end,asign1,comp1);
    }
}

void quickSortHibridizat(int* a, int start, int end,Operation asign2,Operation comp2)
{ ///inca suntem intre limite
    while (start < end)
     { ///pentru numar de elem mai mici decat pragul aplicam insertionsort
         if (end-start < 30)
         {
             insertion_sort(a, start, end,asign2,comp2);
             break;
         }
         else
         { ///else aplicam quicksort, pe sir cu numar de elemente mai mare decat prag
             ///fixam pivot
             int pivot = partition(a, start, end,asign2,comp2);

             if (pivot-start < end-pivot)
             {
                 ///mergem pe prima parte
                 quickSortHibridizat(a, start, pivot - 1,asign2,comp2);
                 start = pivot + 1;
             }
             else
             {
                  ///mergem pe a doua parte
                 quickSortHibridizat(a, pivot + 1, end,asign2,comp2);
                 end = pivot-1;
             }
         }

     }
}

void quickSortHibridizatPRAG(int* a, int start, int end,int prag,Operation asign,Operation comp)
{ ///inca suntem intre limite
    while (start < end)
    {
        ///pentru numar de elem mai mici decat pragul aplicam insertionsort
        if (end-start < prag)
        {
            insertion_sort(a, start, end,asign,comp);
            break;
        }
        else
        {///else aplicam quicksort, pe sir cu numar de elemente mai mare decat prag
            ///fixam pivot
            int pivot = partition(a, start, end,asign,comp);
            if (pivot-start < end-pivot)
            {
                ///mergem pe prima parte
                quickSortHibridizatPRAG(a, start, pivot-1,prag,asign,comp);
                start = pivot + 1;
            }
            else
            {
                ///mergem pe a doua parte
                quickSortHibridizatPRAG(a, pivot+1, end,prag,asign,comp);
                end = pivot-1;
            }
        }
    }
}
void demoBinaryTree()
{
    int a[]= {5,3,2,1,6,8,9};
    int size = sizeof(a)/sizeof(a[0]);

    Operation opps = prof.createOperation("operationsIgnore", size);

    Node* root = build_BinaryTREE(a,0,size);
    printf("Parcurgerea recursiva a arborelui:\n");
    inOrder(root,opps);
    printf("\nParcurgerea iterativa a arborelui:\n");
    inOrderStiva(root,opps);
}

void demoHibridizareQuicksort()
{
    int quick[]= {92,33,21,3,1,4,11,4,6,44,67,88,5,9,17};
    int quickH[]= {92,33,21,3,1,4,11,4,6,44,67,88,5,9,17};

    int size = sizeof(quick)/sizeof(quick[0]);
    Operation asigns = prof.createOperation("AssignmentsIGNORE", size);
    Operation comps = prof.createOperation("ComparisonsIGNORE", size);

    printf("QuickSort Hibridizat:\n");
    quickSortHibridizat(quickH,0,size-1,asigns,comps);
    printeaza_vector(quickH,size);
    printf("QuickSort:\n");
    quickSort(quick,0,size-1,asigns,comps);
    printeaza_vector(quick,size);
}

void perf_Recursiv_VS_Iterativ()
{
    for (int size=STEP_SIZE; size<=MAX_SIZE; size+=STEP_SIZE)
    {
        int vector[size];
        FillRandomArray(vector,size,10,50000,false,UNSORTED);

        Operation opps1 = prof.createOperation("OperatiiIterativ", size);
        Operation opps2 = prof.createOperation("OperatiiRecursiv", size);

        Node *root = build_BinaryTREE(vector,0,size);
        ///numaram numai operatiile de afisare a cheii
        inOrder(root,opps1);
        inOrderStiva(root,opps2);

    }
    prof.createGroup("Total_Operatii","OperatiiIterativ","OperatiiRecursiv");
    prof.showReport();
}

void perf_QuickSortHibridizat()
{
    for (int size=STEP_SIZE; size<=MAX_SIZE; size+=STEP_SIZE)
    {
        int vector[size],vectorSame[size];

            Operation asigns1 = prof.createOperation("Assignments1", size);
            Operation comps1 = prof.createOperation("Comparisons1", size);

            Operation asigns2 = prof.createOperation("Assignments2", size);
            Operation comps2 = prof.createOperation("Comparisons2", size);

        for (int i=0; i<NR_TESTS; i++)
        {
            FillRandomArray(vector,size,10,50000,false,UNSORTED);
            acelasi_vector(vector,vectorSame,size);

            quickSort(vector,0,size-1,asigns1,comps1);
            quickSortHibridizat(vectorSame,0,size-1,asigns2,comps2);
        }
}
    prof.divideValues("Assignments1",NR_TESTS);
    prof.divideValues("Comparisons1",NR_TESTS);
    prof.divideValues("Assignments2",NR_TESTS);
    prof.divideValues("Comparisons2",NR_TESTS);

    prof.addSeries("QuickSort", "Assignments1", "Comparisons1");
    prof.addSeries("QuickSortHibridizat", "Assignments2", "Comparisons2");
    prof.createGroup("QUICKSORT&Q.HIBRID","QuickSort","QuickSortHibridizat");

    prof.showReport();
}

void timer_QuickSortHibridizat()
{
    for (int size=500; size<=MAX_SIZE; size+=STEP_SIZE)
    {
        Operation asigns = prof.createOperation("Assignments", size);
        Operation comps = prof.createOperation("Comparisons", size);

        int vector[size], vectorSame[size];
        FillRandomArray(vector, size, 10, 50000, false, UNSORTED);
        acelasi_vector(vector, vectorSame, size);

        prof.startTimer("quickSort",size);
        for (int t = 0; t < NR_TESTS; t++)
        {
            quickSort(vector,0,size-1,asigns,comps);
        }
        prof.stopTimer("quickSort",size);

        prof.startTimer("quickSortHibridizat",size);
        for (int t = 0; t < NR_TESTS; t++)
        {
            quickSortHibridizat(vectorSame,0,size-1,asigns,comps);
        }
        prof.stopTimer("quickSortHibridizat",size);
        prof.createGroup("totalTime","quickSort","quickSortHibridizat");
    }
    prof.showReport();
}

void perf_QuickSortHibridizatPRAG()
{
    int vector[MAX_SIZE];

    for (int prag = 0; prag < 50; prag++)
    {
        for (int nr = 0; nr < NR_TESTS; nr++)
        {
            Operation asigns = prof.createOperation("Assignments", prag);
            Operation comps = prof.createOperation("Comparisons", prag);
            FillRandomArray(vector, MAX_SIZE, 10, 50000, false, UNSORTED);
            quickSortHibridizatPRAG(vector, 0, MAX_SIZE-1, prag, asigns, comps);
        }
    }
    prof.divideValues("Assignments",NR_TESTS);
    prof.divideValues("Comparisons",NR_TESTS);
    prof.addSeries("QuickSortPRAG", "Assignments", "Comparisons");
    prof.showReport();
}

int main()
{
    ///alternativ ca au chestii comune
   // demoBinaryTree();
    demoHibridizareQuicksort();
    //perf_Recursiv_VS_Iterativ();
    //perf_QuickSortHibridizat();
    //timer_QuickSortHibridizat();
    //perf_QuickSortHibridizatPRAG();
}
