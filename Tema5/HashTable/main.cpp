#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"
#define nr_teste 5

/**
 *  Ciochina Catalina-Andreea
 *  Grupa 30228- semigrupa 1
 *
 *      Am implementat operații de inserare, cautare si stergere dintr-o tabela de dispersie ce folosește
 *  adresarea deschisa cu verificare pătratică.
 *      Toate operatiile folosesc functia de hash h(id,i) = (h’(id)+c1*i*i+ c2*i) mod n , unde n este un
 *  numar prim <=> cu scopul sa fie evitat cazul repetitiei acelorasi resturi la infinit, sa existe cat
 *  mai multe resturi posibile( in acest caz n!=prim nu ar gasi loc deoarece ar accesa de fiecare data
 *  aceleasi pozitii deja ocupate anterior). Am ales c1=c2=1
 *      Cu cat factorul de umplere creste, cu atat efortul creste proportional, ceea ce este normal,
 *  deoarece spatiul de insertie scade. Efortul mediu la negasite este mult mai mare decat cel la gasite,
 *  lucru normal deoarece la negasite tinde sa caute in toate locurile in care s-ar putea afla elem -> O(n),
 *  asta in cazul in care nu gaseste vreun loc gol, unde ar fi trebuit sa se gaseasca elementul cautat
 *
 * Asa arata tabelul in consola, se mai modifica de la generarea random si de la utilizarea fct rand():
 *
 * Factor_Umplere:0.80|Efort_mediu_gasite:1.94|Efort_maxim_gasite:33|Efort_mediu_negasite:4.65|Efort_maxim_negasite:62
 * Factor_Umplere:0.85|Efort_mediu_gasite:2.18|Efort_maxim_gasite:58|Efort_mediu_negasite:6.82|Efort_maxim_negasite:74
 * Factor_Umplere:0.90|Efort_mediu_gasite:2.55|Efort_maxim_gasite:67|Efort_mediu_negasite:12.35|Efort_maxim_negasite:125
 * Factor_Umplere:0.95|Efort_mediu_gasite:3.54|Efort_maxim_gasite:94|Efort_mediu_negasite:31.93|Efort_maxim_negasite:207
 * Factor_Umplere:0.99|Efort_mediu_gasite:5.24|Efort_maxim_gasite:265|Efort_mediu_negasite:102.75|Efort_maxim_negasite:792
 *
 * AFTER DELETE
 * Factor_Umplere_inainte_de:0.99
 * Factor_Umplere_dupa:0.80
 * Efort_mediu_gasite:1.59
 * Efort_maxim_gasite:23
 * Efort_mediu_negasite:4.34
 * Efort_maxim_negasite:33
 **/

Profiler prof("HashTable");

typedef struct Element {

    int id;
    char nume[30];
    int occupied;

}Element;

typedef struct HashTable{

    Element *data;
    unsigned size;

}HashTable;

HashTable* create_hashTable(unsigned size){

    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    ///nr total de elem adaugate in hashTable
    hashTable->size=size;

    hashTable->data= (Element*)(calloc(size, sizeof(Element)));
    hashTable->data->id =0;
    hashTable->data->occupied =0;

    strcpy(hashTable->data->nume, "empty");
    return hashTable;
}
void insert_hashTable(HashTable *hashTable, int id, char* nume){

    ///adresare deschisa cu verificare patratica
    /// h'(id) = id % n;
   int index=id % hashTable->size;

       for(int i=0;i<hashTable->size;++i){
           /// functia finala de hash este
           /// h(id,i) = (h’(id)+c1*i*i+ c2*i) mod n
           /// unde c1 = c2 = 1
           int newIndex=(index+ i * i + i )% hashTable->size;
            /// daca nu este ocupat, punem valorile
           if(hashTable->data[newIndex].occupied==0){

               hashTable->data[newIndex].id=id;
               strcpy(hashTable->data[newIndex].nume,nume);
               hashTable->data[newIndex].occupied=1;
               return;
           }
       }

}
int search_hashTable (HashTable *hashTable, int id, char* nume){
        int index = id % hashTable->size;

        for (int i = 0; i < hashTable->size; ++i) {

            int newIndex = (index + i * i + i) % hashTable->size;
            ///daca este ocupat, incercam sa vedem daca este elem cautat
            if (hashTable->data[newIndex].occupied == 1) {

                if (hashTable->data[newIndex].id==id)
                {
                    if(strcmp(hashTable->data[newIndex].nume, nume) == 0 )
                        return 1;
                }
            }else
                ///daca este lieber unde ar trebui sa fie elem, clar nu gasim elem cautat
                return 0;
        }
        return 0;
}
int search_hashTable_effort (HashTable *hashTable, int id, char* nume, int* effort ){

    int index = id % hashTable->size;

    for (int i = 0; i < hashTable->size; ++i) {

        int newIndex = (index + i * i + i) % hashTable->size;
        if (hashTable->data[newIndex].occupied == 1) {
           *effort+=1;
            if (strcmp(hashTable->data[newIndex].nume, nume) == 0 && hashTable->data[newIndex].id==id)
                return 1;

        ///daca nu e ocupat, clar nu gasim elementul in tabela
        }else
            return 0;
           }
    return 0;
}
void deleteFrom_HashTable(HashTable *hashTable, int id, char* nume){

        int index=id % hashTable->size;
            for (int i = 0; i < hashTable->size; ++i) {

                int newIndex = (index + i * i + i ) % hashTable->size;
                    ///eliberam locul din tabela care era ocupat de elem, restabilind starea initiala
                    if(hashTable->data[newIndex].id==id && strcmp(hashTable->data[newIndex].nume, nume) == 0 ) {

                        hashTable->data[newIndex].occupied=0;
                        strcpy(hashTable->data[newIndex].nume,"empty");
                        hashTable->data[newIndex].id=0;
                    }
            }
}
void free_hashTable(HashTable *hashTable){
    if(hashTable==NULL)
        return;
    free(hashTable->data);
    free(hashTable);

}
void print_hashTable(HashTable *hashTable){

    for(int i=0; i < hashTable->size; ++i){
        printf("%d : ",i);
        if(hashTable->data[i].occupied == 1) {
            printf(" %d ", hashTable->data[i].id);
            printf(" %s\n",hashTable->data[i].nume);
        }
        else {
            printf("empty\n");
        }
    }
}
void demo(){
    /// factor de umplere 95%
    ///dimensiunea tabelei o tinem ca un nr prim ca sa fie resturi cat mai variate, sa incercam sa gasim un loc la fiecare
    HashTable* table = create_hashTable(11);
    int keys[] = {19,36,5,21,4,26,14,10,23,25};
    int nr_elem = sizeof(keys)/sizeof (int);
    char* nume[] ={"mere", "pere","prune", "piersici","gutui","cirese","smochine","struguri","capsuni","kiwi"};

    for(int i=0 ; i< nr_elem; ++i){
        insert_hashTable(table,keys[i],nume[i]);
    }
    print_hashTable(table);
    printf("\nSEARCH: %d ",search_hashTable(table,21,"piersici"));
    printf("%d ",search_hashTable(table,19,"mere"));
    printf("%d\n\n",search_hashTable(table,19,"kiwi"));

    deleteFrom_HashTable(table,19,"kiwi");
    deleteFrom_HashTable(table,19,"mere");
    deleteFrom_HashTable(table,36,"kiwi");
    deleteFrom_HashTable(table,36,"pere");
    printf("Dupa DELETE: \n");
    print_hashTable(table);
}
void perf_search(){

 srand(time(0));

    /// alfa = nr_elem_introduse / N_dim_tabela
    double alfa[6]={0.8,0.85, 0.9, 0.95, 0.99};

    for(int i=0; i < 5; ++i){
     ///avem 5 cazuri in care vrem sa vedem comportarea operatiilor pe tabela

        HashTable *table;
        int n, efortMaximGasite= 0,efortMaximNegasite = 0,efortTotalGasite = 0,efortTotalNegasite = 0;

        for(int t= 0; t < nr_teste; ++t) {
            ///am creat tabela
            table = create_hashTable(10007);

            /// nr elemente introduse in tabela n=alfa*N
            n = alfa[i] * table->size;

            int *keys = (int *) (calloc(10007 + 1500, sizeof(int)));
            ///n + 1500 pe care nu le introducem in tabela, dar le cautam
            FillRandomArray(keys, n + 1500, 0, 40000, true, UNSORTED);

            ///populam tabela
            for (int j = 0; j < n; ++j) {
                insert_hashTable(table, keys[j], "*element_inserat*");
            }
            // print_hashTable(table);

            ///1500 elemente pe care ar trebui sa le gasim in tabela
            for (int k = 0; k < 1500; ++k) {

                /// am ales random elementul pe care sa il caut
                int poz = rand() % (n + 1);
                int efortGasite = 0;
                search_hashTable_effort(table, keys[poz], "*element_inserat*", &efortGasite);
                efortTotalGasite = efortTotalGasite + efortGasite;
                if (efortMaximGasite < efortGasite)
                    efortMaximGasite = efortGasite;
            }
            ///1500 elemente pe care sigur nu le gasim in tabela
            for (int k = n; k < n + 1500; ++k) {

                int efortNegasite = 0;
                search_hashTable_effort(table, keys[k], "*element_inserat*", &efortNegasite);
                efortTotalNegasite = efortTotalNegasite + efortNegasite;
                if (efortMaximNegasite < efortNegasite)
                    efortMaximNegasite = efortNegasite;
            }
            free_hashTable(table);
        }

        ///impartim la nr de teste, efortul total
        efortTotalGasite/=5;
        efortTotalNegasite/=5;

        ///efort mediu = efort_total / nr_elemente
        printf("Factor_Umplere:%.2f|Efort_mediu_gasite:%.2f|Efort_maxim_gasite:%d|Efort_mediu_negasite:%.2f|Efort_maxim_negasite:%d\n",alfa[i],efortTotalGasite/1500.0,efortMaximGasite,efortTotalNegasite/1500.0,efortMaximNegasite);
    }
}
void perf_search_after_delete(){

    srand(time(0));

    /// alfa = nr_elem_introduse / N_dim_tabela
    double alfa1 = 0.99,alfa2 = 0.8;
    ///nr elemente introduse in tabela n=alfa*N
    int nr_elem_total = alfa1 * 10007;
    int nr_elem_after_delete = alfa2 * 10007;
    ///cate elemente trebuie sa stergem
    int nr_elem_deleted = nr_elem_total - nr_elem_after_delete;
    int efortMaximGasite= 0,efortMaximNegasite = 0,efortTotalGasite = 0,efortTotalNegasite = 0;

    HashTable *table = create_hashTable(10007);
    int *keys = (int *) (calloc(10007 + 1500, sizeof(int)));
    FillRandomArray(keys, nr_elem_total + 1500, 0, 40000, true, UNSORTED);

        ///populam tabela
        for (int j = 0; j < nr_elem_total; ++j) {
            insert_hashTable(table, keys[j], "*element_inserat*");
        }
        // print_hashTable(table);
        ///stergem cele nr_elem_deleted din tabela, id il obtinem random din vectorul de keys
        for (int j = 0; j < nr_elem_deleted; ++j) {
            int poz = rand() % (nr_elem_total + 1);
            deleteFrom_HashTable(table,keys[poz],"*element_inserat*");
        }

        ///cautam in tabela dupa stergere, in prima faza elem pe care ar trebui sa le gasim
        for (int k = 0; k < 1500; ++k) {
            int poz = rand() % ( nr_elem_total + 1);
            int efortGasite = 0;
            search_hashTable_effort(table, keys[poz], "*element_inserat*", &efortGasite);
            efortTotalGasite = efortTotalGasite + efortGasite;
            if (efortMaximGasite < efortGasite)
                efortMaximGasite = efortGasite;
        }
        ///si pe care sigur nu le gasim in tabela
        for (int k = nr_elem_total; k < nr_elem_total + 1500; ++k) {
            int efortNegasite = 0;
            search_hashTable_effort(table, keys[k], "*element_inserat*", &efortNegasite);
            efortTotalNegasite = efortTotalNegasite + efortNegasite;
            if (efortMaximNegasite < efortNegasite)
                efortMaximNegasite = efortNegasite;
        }
 printf("AFTER DELETE\nFactor_Umplere_inainte_de:%.2f\nFactor_Umplere_dupa:%.2f\nEfort_mediu_gasite:%.2f\nEfort_maxim_gasite:%d\nEfort_mediu_negasite:%.2f\nEfort_maxim_negasite:%d\n",alfa1,alfa2,efortTotalGasite/1500.0,efortMaximGasite,efortTotalNegasite/1500.0,efortMaximNegasite);

}
int main(){
   demo();
   perf_search();
   perf_search_after_delete();
   return 0;
}
