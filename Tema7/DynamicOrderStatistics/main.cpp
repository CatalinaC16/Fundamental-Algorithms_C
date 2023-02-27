#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define TESTE 5
#define STEP_SIZE 100
#define MAX_SIZE 10000

/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *       Am implementat un arbore binar de cautare perfect echilibrat cu ajutorul unei structuri ce are un pointer spre copilul drept, un
 *  pointer spre copilul stang, valoarea nodului si un camp size ce contine dimensiunea subarborelui ce are nodul ca radacina.
 *       Ca operatii asupra arborelui am implementat: construirea arborelui BUILD, dintr-un vector ordonat crescator prin metoda DivideEtImpera,
 *  cautarea nodului cu a i-a cea mai mica cheie - OS_Select si stergerea unui nod cu pastrarea proprietatilor de bst OS_Delete ->care implica
 *  si cautarea elemtului, apoi stergerea si restabilirea legatirilor si a dimensiunilor afectate.
 *      Am facut si analiza in cazul mediu statistic a complexitatii operatiilor de cautare si stergere din arborele binar de cautare
 *  perfect echilibrat. Complexitatea operatiilor este O(nlogn) in cazul mediu statistic, in worst ar trebui sa fie O(n^2).
 *
 */
Profiler prof("DOStatistics");

int v[]= {1,2,3,4,5,6,7,8,9,10,11};
int n = sizeof(v)/sizeof(v[0]);

typedef struct Node
{
    Node* stanga;
    Node* dreapta;
    int data;
    int size;

} Node;

Node* creareNode(int data)
{
    ///alocam memorie nodului
    Node* node= (Node*)calloc(1,sizeof(Node));
    ///stabilim pointerii la copii sai pe NULL
    node->stanga = NULL;
    node->dreapta = NULL;
    ///asignam valoarea
    node->data = data;
    /// nr de noduri/size-ul fiecarui node in faza de creare
    node->size = 1;
    return node;

}

Node* construire(int stanga, int dreapta,Operation asign, Operation comp)
{
    ///conditia de oprire
    if(stanga>=dreapta)
    {
        return NULL;
    }
    else
    {

        ///sirul este ordonat crescator, luam mediana sirului si
        ///construim root-ul respectivului subtree(recursiv)
        int medPoz = (stanga + dreapta) / 2;
        int mediana = v[medPoz];

        Node *root = creareNode(mediana);
        /// la copilul stanga luam mediana din prima jumatate a sirului/subsirului
        root->stanga = construire(stanga, medPoz,asign,comp);
        /// la copilul dreapta luam mediana din a doua jumatate a sirului/subsirului
        root->dreapta = construire(medPoz + 1, dreapta,asign,comp);


        ///pentru fiecare node creat,prin recursiviatea se merge de
        ///jos in sus si size pentru nodul curent este dim pe dreapta + dim pe stanga
        comp.count();
        if (root->stanga != NULL)
        {
            asign.count();
            root->size += root->stanga->size;
        }

        comp.count();
        if (root->dreapta != NULL)
        {
            asign.count();
            root->size += root->dreapta->size;
        }
        ///daca copii exista, pentru fiecare root al unui subtree size = sizeDreapta+sizeStanga
        return root;
    }
}
Node* build(int n,Operation asign, Operation comp)
{
    ///construim arborele binar de cautare perfect echilibrat
    ///cu chei
    Node* root = construire(0,n,asign, comp);
    return root;
}

void prettyPrint(Node* root, int nivel)
{
    ///conditia de intoarcere/oprire in recursivitate
    if(root == NULL)
    {
        return;
    }
    else
    {
        nivel+=1;
        ///subarborele dreapta
        prettyPrint(root->dreapta,nivel);

        ///identarea pe nivel
        for(int i=0; i<nivel-1; ++i)
        {
            printf("      ");
        }
        ///am pus valoarea nodului, urmata de size-ul nodului respectiv
        printf("%d sz=%d\n", root->data, root->size);

        ///subarborele stanga
        prettyPrint(root->stanga,nivel);

    }
}

Node* get_OS_select(Node* root, int i,Operation asign, Operation comp)
{

    comp.count();
    ///in cazul in care nodul nu este alocat
    if(root== NULL)
        return NULL;

    /// in cazul general, pe noduri frunza
    int sz=1;

    comp.count();
    ///daca nodul are subtree stang
    if (root->stanga != NULL)
    {
        asign.count();
        ///stabilim sz
        sz = root->stanga->size + 1;
    }

    ///in cazul in care este egal cu i, inseamna ca este chiar nodul cautat
    ///elementul cu a i-a cea mai mica cheie
    if(sz == i)
    {
        return root;
    }
    ///daca i<sz mergem pe subarbore stanga
    else if (sz > i)
    {
        return get_OS_select(root->stanga,i,asign,comp);
    }
    ///daca i>sz mergem pe subarbore dreapta
    else return get_OS_select(root->dreapta,i-sz,asign,comp);
}

int OS_select(Node* root, int i,Operation asign, Operation comp)
{
    asign.count();
    ///cautam nodul
    Node* node = get_OS_select(root,i,asign,comp);
    comp.count();
    ///in cazul in care nu a fost gasit sau este out of borders
    if(node == NULL || i > n || i<0)
        return 0;
    else
        ///daca nu returnam valoarea
        return node->data;
}

Node* minim(Node* root,Operation asign,Operation comp)
{

///root este radacina subarborelui din dreapta
    asign.count();
    Node* node = root;

    comp.count(2);
    ///fiind bst cautam minimul in subarborele din stanga, cel mai din stanga node
    ///este minimul(frunza)
    while (node != NULL && node->stanga != NULL)
    {

        node = node->stanga;
        asign.count();
    }
    return node;
}


Node* deleteNode(Node* root, Node* deleted,Operation asign, Operation comp)
{

    comp.count();
    ///nodul nu a fost gasit
    if(root == NULL)
        return NULL;
    else
    {
        comp.count();
        if (deleted->data < root->data)
        {
            ///in cazul in care valoarea nodului de sters este mai mica decat valoarea radacinii
            ///mergem pe subarborele stang
            root->stanga = deleteNode(root->stanga, deleted, asign, comp);
            asign.count();
        }
        else if (deleted->data > root->data)
        {
            ///altfel,daca este mai mare mergem pe subarborele drept
            root->dreapta = deleteNode(root->dreapta, deleted, asign, comp);
            asign.count();
        }
        else
        {
            ///am ajuns la nodul de sters, caruia trebuie sa ii restabilim legaturile
            comp.count();
            if (root->stanga == NULL)
            {
                ///daca nu are subarbore stang
                asign.count();
                Node *next = root->dreapta;
                free(root);
                return next;

            }
            else if (root->dreapta == NULL)
            {
                ///daca nu are subarbore drept
                asign.count();
                Node *next = root->stanga;
                free(root);
                return next;
            }
            ///are si subarbore drept si subarbore stang
            asign.count(3);
            ///cautam cel mai din stanga elem din subarborele din dreapta
            Node *min = minim(root->dreapta, asign, comp);
            root->data = min->data;
            ///mergem in continuare cu restabilirea legaturilor, repetarea principiului
            root->dreapta = deleteNode(root->dreapta, min, asign, comp);
        }
    }
    asign.count();
    root->size = 1;
    ///restabilim si dimensiunea nodurilor pe care le-am afectat

    comp.count(2);
    if (root->stanga != NULL)
        root->size += root->stanga->size;
    if (root->dreapta != NULL)
        root->size += root->dreapta->size;

    return root;
}

Node* OS_delete(Node* root, int i,Operation asign,Operation comp)
{

    if(i<n)
    {
        asign.count();
        ///cautam nodul de sters
        Node *deleted = get_OS_select(root, i,asign,comp);
        if(deleted)
        {
            int data = deleted->data;
            asign.count();
            root = deleteNode(root, deleted, asign, comp);
            printf("Am sters %d\n", data);
            return root;
        }
        else
            ///in cazul in care nu l-am gasit
            return NULL;
    }
    else
    {
        return NULL;
    }

}
void demo()
{
    Operation asign = prof.createOperation("Assignsments", n);
    Operation comp = prof.createOperation("Comparisons", n);
    ///construim arborele
    Node* root = build(n,asign,comp);
    printf("Arbore binar de cautare perfect echilibrat: node/size\n");
    prettyPrint(root,0);


    printf("OS SELECT\n");
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",1, OS_select(root,1,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",2, OS_select(root,2,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",3, OS_select(root,3,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",4, OS_select(root,4,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",5, OS_select(root,5,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",6, OS_select(root,6,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",7, OS_select(root,7,asign,comp));
    printf("Elementul cu a %d-a cea mai mica val este: %d\n",9, OS_select(root,9,asign,comp));

    printf("\nOS DELETE\n");
    ///am facut ca in functia delete sa faca si
    /// cautarea nodului respectiv
    root = OS_delete(root,9,asign,comp);
    root = OS_delete(root,3,asign,comp);
    root = OS_delete(root,2,asign,comp);
    //root = OS_delete(root,6,asign,comp);
    // root = OS_delete(root,16);
    prettyPrint(root,0);
}

void perf()
{

    srand(time(0));

    for (int i=0; i<TESTE; ++i)
    {

        for (int size=STEP_SIZE; size<=MAX_SIZE; size+=STEP_SIZE)
        {

            int vector[size];
            ///construim vectorul crescator de keys de la 1 la n=size
            FillRandomArray(vector, size, 1, size, TRUE, ASCENDING);

            Operation asigns = prof.createOperation("Assignments", size);
            Operation comps = prof.createOperation("Comparisons", size);

            ///construim binary search tree ul perfect echilibrat
            Node* root = build(size, asigns, comps);
            int aux = size;
            ///repetam de n ori
            for (int i=0; i<size; ++i)
            {
                ///index selectat aleator din intervalul [0,size]
                int ii = rand() % aux;
                while(ii > aux)
                {
                    ii = rand() % aux;
                    if(ii < aux)
                        break;
                }
                if (rand() % 2 == 0)
                {
                    OS_select(root, ii, asigns, comps);
                }
                else
                {
                    OS_delete(root, ii, asigns, comps);
                    aux--;
                }
            }
        }
    }
    prof.divideValues("Assignments",TESTE);
    prof.divideValues("Comparisons",TESTE);
    prof.addSeries("Total", "Assignments", "Comparisons");
    prof.showReport();
}

int main()
{
///alternativ

   demo();
   //perf();
    return 0;
}
