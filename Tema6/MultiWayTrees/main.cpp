#include <stdio.h>
#include <stdlib.h>
#define NRMAXCOPII 10
/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *      Am implementat 3 reprezentari ale unui arbore multicai si transformari intre aceste reprezentari.
 *  Pentru fiecare reprezentare am facut o structura de date :
 *      Pentru reprezentarea parinte am definit o structura de tipul NodeParinte, care are 2 campuri (data, parinte),
 *  in data puneam val indexului nodului = i si in parinte valoarea indexului parintelui=vec[i],puteam sa dau si vectorul direct
 *  la pretty print, dar asa m-am gandit prima data (si am mers in continuare sa fie cat mai apropiata si  prima reprezentare de
 *  reprezentarea unui arbore) si am implementat si o transformare din vector intr-un vector de structuri de tipul NodeParinte
 *      Pentru reprezentarea arbore multicai am definit o structura de tipul NodeMulti, care contine indexul nodului, al catelea copil(cati
 *  au fost adaugati pentru fiecare Node si un vector de copii(max 10) de tipul NodeMulti. Aici a fost nevoie de transformarea din NodeParinte
 *  in NodeMulti, in care am parcurs vec de NodeParinte si in fiecare caz creez nodul parinte si merg in adancime recursiv sa ii caut copii.
 *      Pentru reprezentarea binara am definit o structura de tipul NodeBinar, care contine valoare nod = indexul si doua noduri de tipul
 *  NodeBinar(referinta) unul catre primul copil si al doilea catre fratele din dreapta. In transformarea din NodeMulti in NodeBinar, cream
 *  noul frate si mergem in adancime cu parinte, restabilim parintele si fratele dreapta, apoi in adancime cu frate dreapta, restabilim
 *  parintele si fratele dreapta. Incepem de la primul copil al radacinii Multi si primul copil ca parinte si primul copil al parintelui,
 *  ca frate dreapta.
 *      Nu am avut nevoie de memorie aditionala pe langa cea pe care am alocat-o pentru fiecare structura( sau vector) pe care le-am declarat,
 *  am facut totul in place.
 *  Facand majoritatea legaturilor recursiv cel mai probabil ma apropii de complexitate ~= O(n).
 *      Pentru fiecare reprezentare am facut si un pretty print al arborelui.
 *  **/
typedef struct NodeParinte{

    int data;
    int parinte;

}NodeParinte;

typedef struct NodeMulti{

    int data;
    int nr_copil;
    NodeMulti *copii[NRMAXCOPII];

}NodeMulti;

typedef struct NodeBinar{

    int data;
    NodeBinar* primulCopil;
    NodeBinar* frateDreapta;

}NodeBinar;

void printPretty(int data,int nivel){

    ///in functie de nivel face o identare a valorii
    for(int j=0; j < nivel; ++j){
        printf("  ");
    }
    printf("%d\n",data);
}

NodeParinte* creareNodePari(int data, int parinte){

    ///alocam memorie node de tipul NodeParinte
    NodeParinte* Node = (NodeParinte*)calloc(1,sizeof(NodeParinte));
    ///asignam valorile din vectorul de tati
    Node->data = data;
    Node->parinte =parinte;
    return Node;
}
/// din vectorul de tati intr-un vector de noduri, de tipul NodeParinte
NodeParinte** transformarea0(int *vectorParinti, int size){

    ///alocam un vector de noduri de tipul NodeParinte si le cream
    NodeParinte** Node = (NodeParinte **) malloc(size * sizeof(NodeParinte*));
    for (int i = 0; i < size; ++i)
    {
        Node[i] = (NodeParinte*) malloc(sizeof(NodeParinte));
        Node[i] = creareNodePari(i,vectorParinti[i]);
    }

    return Node;
}

void reprezentareaParinte(NodeParinte** pari,int parinte, int nivel, int size){

    if(pari == NULL )
        return;
    else{
        for(int i=0; i< size; ++i) {
            ///verificam daca sunt noduri cu un parinte si mergem "in adancime" cu fiecare parinte, recursiv
            if (pari[i]->parinte == parinte) {
                ///afisam valoarea din nodul de la nivelul la care ne aflam
                printPretty(pari[i]->data, nivel);
                ///mergem in adancime pana cand nu mai sunt noduri si marim nivelul
                reprezentareaParinte(pari, pari[i]->data, nivel + 1, size);
            }
        }
    }
}

NodeMulti* creareNodeMulti(int data){

    ///alocam memorie node
    NodeMulti* Node = (NodeMulti*)calloc(1,sizeof(NodeMulti));
    ///asignam valorile de inceput ale unui node de tipul NodeMulti
    Node->data = data;
    Node->nr_copil = 0;
    ///fiecare node copil asignam la NULL
    for(int i=0; i < NRMAXCOPII ; ++i){
        Node->copii[i]=NULL;
    }
    return Node;
}

NodeMulti* transformarea1(NodeParinte**pari,NodeMulti* multi, int size){

    int numarCopil;
    ///in cazul in care nu este creat nodul radacina
        if(multi == NULL){
            multi = creareNodeMulti(-1);
            transformarea1(pari,multi,size);
        }
        else {
            for (int i = 0; i < size; ++i) {
                /// daca gasim un copil
                if (pari[i]->parinte == multi->data) {

                    numarCopil = multi->nr_copil;
                    ///cream un nou copil in vectorul de copii ai acelui node
                    multi->copii[numarCopil] = creareNodeMulti(pari[i]->data);
                    ///mergem in adancime
                    transformarea1(pari,multi->copii[numarCopil],size);
                    multi->nr_copil++;
                }
            }
        }
    return multi;
}

void reprezentareaMultiCai(NodeMulti *multi, int nivel){

    if(multi == NULL)
        return;
    else {
        for (int i = 0; i < multi->nr_copil; ++i) {
            ///afisam valoarea din nodul de la nivelul la care ne aflam
            printPretty(multi->copii[i]->data, nivel);
            ///mergem in adancime pana cand nu mai sunt noduri si marim nivelul la fiecare pas
            reprezentareaMultiCai(multi->copii[i], nivel + 1);
        }
    }
}

NodeBinar* creareNodeBinar(int data){
    ///alocam nodul
    NodeBinar* Node = (NodeBinar*)calloc(1,sizeof(NodeBinar));
    ///asignam valorile initiale
    Node->data = data;
    Node->frateDreapta = NULL;
    Node->primulCopil = NULL;
    return Node;
}

NodeBinar* transformarea2(NodeMulti* parinte,NodeMulti* frateDreapta, int i){

    ///cream node pentru urmatorul frate
    NodeBinar* node = creareNodeBinar(frateDreapta->data);
    /// in adancime cu parinte, restabilim parintele si fratele dreapta
    if (parinte->nr_copil > i+1 && parinte != NULL) {
        node->frateDreapta = transformarea2(parinte, parinte->copii[i+1], i+1);
    }
    /// in adancime frate dreapta, restabilim parintele si fratele dreapta
    if (frateDreapta->nr_copil > 0 && frateDreapta != NULL) {
        node->primulCopil = transformarea2(frateDreapta, frateDreapta->copii[0], 0);
    }
    return node;

}

void reprezentareaBinara(NodeBinar* binar, int nivel)
{
    if(binar == NULL)
        return;
    else{
        /// facem o parcurgere in preordine
        printPretty(binar->data, nivel);
        ///primul copil
        if (binar->primulCopil != NULL) {
            reprezentareaBinara(binar->primulCopil, nivel + 1);
        }
        ///urmatorul frate
        if (binar->frateDreapta != NULL) {
            reprezentareaBinara(binar->frateDreapta, nivel);
        }
    }
}

void try_demo(){
    int vec_parinti[]={0,2, 7, 5, 2, 7, 7,-1, 5, 2};
    int size = sizeof(vec_parinti)/sizeof(int);
    NodeMulti *multi= NULL;
    NodeMulti *MultiWayTree;
    NodeParinte **pari = NULL;
    NodeBinar* bin;

    ///reprezentarea parinte + transformare0 => din array in vector de structuri
    pari = transformarea0(vec_parinti,size);
    reprezentareaParinte(pari,-1,0,size);
    printf("\n\n");

    ///reprezentare arbore multicai + transformare1 => reprezentarea parinte in reprezentarea
    ///arbore multicai
    MultiWayTree = transformarea1(pari,multi,size);
    reprezentareaMultiCai(MultiWayTree,0);
    printf("\n\n");

    ///reprezentarea binara + transformare1 => reprezentarea multicai in reprezentarea binara
    ///incepem de la primul copil al radacinii
    bin = creareNodeBinar(MultiWayTree->copii[0]->data);
    ///primul copil ca parinte si primul copil al parintelui, ca frate dreapta
    bin->primulCopil =  transformarea2(MultiWayTree->copii[0], MultiWayTree->copii[0]->copii[0], 0);
    reprezentareaBinara(bin,0);
    printf("\n\n");
}
int main() {
   try_demo();

   return 0;
}
