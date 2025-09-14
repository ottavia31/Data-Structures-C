#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_INGREDIENTI 20
#define DIM_HASH_TABLE1 16879
#define DIM_HASH_TABLE2 317 

typedef struct {
    char nome[MAX_NOME];
    int quantita;
} t_ingrediente;

typedef struct {
    char nome[MAX_NOME];
    int num_ingr;
    int peso;
    t_ingrediente ingredienti[MAX_INGREDIENTI];
} t_ricetta;

typedef struct t_nodo_hash {
    t_ricetta* ricetta;
    struct t_nodo_hash* next;
} t_nodo_hash;

typedef struct {
    t_nodo_hash* tabella[DIM_HASH_TABLE1];
    int num_ricette;
} t_ricettario;

unsigned int hash1(const char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {

        hash = ((hash << 5) + hash) + c; 
    }
 
    return hash % DIM_HASH_TABLE1;
}

t_ricettario* crea_ricettario() {
    t_ricettario* ricettario = malloc(sizeof(t_ricettario));
    for (int i = 0; i < DIM_HASH_TABLE1; i++) {
        ricettario->tabella[i] = NULL;
    }
    ricettario->num_ricette = 0;
    return ricettario;
}



void aggiungi_ricetta(t_ricettario* ricettario, t_ricetta* ricetta) {
    unsigned int index = hash1(ricetta->nome);
    t_nodo_hash* nuovo_nodo = malloc(sizeof(t_nodo_hash));


    if (ricettario->tabella[index] == NULL) {
        nuovo_nodo->ricetta = malloc(sizeof(t_ricetta));
        memcpy(nuovo_nodo->ricetta, ricetta, sizeof(t_ricetta));
        nuovo_nodo->next = NULL;
        ricettario->tabella[index] = nuovo_nodo;
    } else {
        t_nodo_hash* current = ricettario->tabella[index];
        while (current->next != NULL) {
            if (strcmp(current->ricetta->nome, ricetta->nome) == 0) {
                printf("ignorato\n");
                free(nuovo_nodo);
                return;
            }
            current = current->next;
        }
        if (strcmp(current->ricetta->nome, ricetta->nome) == 0) {
            printf("ignorato\n");
            free(nuovo_nodo);
            return;
        }
        nuovo_nodo->ricetta = malloc(sizeof(t_ricetta));
        memcpy(nuovo_nodo->ricetta, ricetta, sizeof(t_ricetta));
        nuovo_nodo->next = NULL;
        current->next = nuovo_nodo;
    }

    ricettario->num_ricette++;
    printf("aggiunta\n");
}

t_ricetta* cerca_ricetta(t_ricettario* ricettario, const char* nome_ricetta) {
    unsigned int index = hash1(nome_ricetta);
    t_nodo_hash* curr = ricettario->tabella[index];
    
    while (curr != NULL) {
        if (strcmp(curr->ricetta->nome, nome_ricetta) == 0) {
            return curr->ricetta;
        }
        curr = curr->next;
    }
    
    return NULL;
}

void rimuovi_ricetta(t_ricettario* ricettario, const char* nome_ricetta) {


    unsigned int index = hash1(nome_ricetta);
    t_nodo_hash* current = ricettario->tabella[index];
    t_nodo_hash* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->ricetta->nome, nome_ricetta) == 0) {
            if (prev == NULL) {
                ricettario->tabella[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->ricetta);
            free(current);
            ricettario->num_ricette--;
            printf("rimossa\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    
    printf("non presente\n");
    
}

typedef struct lotto{
    int quantita;
    int scadenza;
    struct lotto* next;
} t_lotto;

typedef struct ingr_magazzino{
    char nome[MAX_NOME];
    t_lotto* lotti;
    int num_lotti;
    struct ingr_magazzino* next;
} t_ingr_magazzino;

typedef struct {
    t_ingr_magazzino* tabella[DIM_HASH_TABLE2];
    int num_ingr;
    int capacita;
} t_magazzino;
unsigned int hash2(const char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {

        hash = ((hash << 5) + hash) + c; 
    }
 
    return hash % DIM_HASH_TABLE2;
}

t_magazzino* crea_magazzino() {
    t_magazzino* magazzino = malloc(sizeof(t_magazzino));
    for (int i = 0; i < DIM_HASH_TABLE2; i++) {
        magazzino->tabella[i] = NULL;
    }
    magazzino->num_ingr = 0;
    return magazzino;
}

t_ingr_magazzino* cerca_ingr_magazzino(t_magazzino* magazzino, const char* nome) {
    unsigned int index = hash2(nome);
    t_ingr_magazzino* ingr = magazzino->tabella[index];
    while (ingr != NULL) {
        if (strcmp(ingr->nome, nome) == 0) {
            return ingr;
        }
        ingr = ingr->next;
    }
    return NULL;
}

t_ingr_magazzino* crea_ingr_magazzino(t_magazzino* magazzino, const char* nome) {
    t_ingr_magazzino* nuovo = malloc(sizeof(t_ingr_magazzino));
    strcpy(nuovo->nome, nome);
    nuovo->lotti = NULL;
    nuovo->num_lotti = 0;
    nuovo->next = NULL;

    unsigned int index = hash2(nome);
    if (magazzino->tabella[index] == NULL) {
        magazzino->tabella[index] = nuovo;
    } else {
        nuovo->next = magazzino->tabella[index];
        magazzino->tabella[index] = nuovo;
    }

    magazzino->num_ingr++;
    return nuovo;
}

void aggiungi_lotto(t_magazzino* magazzino, const char* nome_ingrediente, int quantita, int scadenza) {

    t_ingr_magazzino* ingr = cerca_ingr_magazzino(magazzino, nome_ingrediente);

    if(ingr==NULL){
        ingr = crea_ingr_magazzino(magazzino, nome_ingrediente);
    }

    
    t_lotto* nuovo = malloc(sizeof(t_lotto)); 
    nuovo->quantita = quantita;
    nuovo->scadenza = scadenza;
    nuovo->next = NULL;

    if(ingr->lotti == NULL || scadenza < ingr->lotti->scadenza){
        nuovo->next = ingr->lotti;
        ingr->lotti = nuovo;
    }else{
        t_lotto* curr = ingr->lotti;
        while(curr->next!=NULL && curr->next->scadenza <= scadenza){
            curr = curr->next;
        }
        nuovo->next = curr->next;
        curr->next = nuovo;
    }

    ingr->num_lotti += quantita;
}

void usa_ingrediente(t_ingr_magazzino* ingr, const char* nome, int quantita, int tempo){



    int rimasti = quantita;
    t_lotto* curr = ingr->lotti;
    t_lotto* prev = NULL;

    while ( curr != NULL && rimasti > 0){

        if (curr->scadenza <= tempo){
            if(prev == NULL){
                ingr->lotti = curr->next;
            }else{
                prev->next = curr->next;
            }

            t_lotto* scaduto = curr;
            curr = curr->next;
            ingr->num_lotti -= scaduto->quantita;
            free(scaduto);
        }else if(curr->quantita <= rimasti){
            rimasti -= curr->quantita;
            ingr->num_lotti-=curr->quantita;

            if(prev == NULL){
                ingr->lotti = curr->next;
            }else{
                prev->next = curr->next;
            }

            curr = curr->next;


        }else{
            curr->quantita -=rimasti;
            ingr->num_lotti -= rimasti;
            rimasti=0;
        }

        
    }


}


void rimuovi_lotti_scaduti(t_magazzino* magazzino, int tempo) {
    for (int i = 0; i < DIM_HASH_TABLE2; i++) {
        t_ingr_magazzino* ingr = magazzino->tabella[i];
        while (ingr != NULL) {
            t_lotto* curr = ingr->lotti;
            t_lotto* prev = NULL;

            while (curr != NULL && curr->scadenza <= tempo) {
                ingr->num_lotti -= curr->quantita;
                t_lotto* scaduto = curr;
                curr = curr->next;
                free(scaduto);
                
                if (prev == NULL) {
                    ingr->lotti = curr;
                } else {
                    prev->next = curr;
                }
            }
            
            ingr = ingr->next;
        }
    }
}


typedef struct rifornimento{
    char nome_ingrediente[MAX_NOME];
    int quantita;
    int scadenza;
    struct rifornimento * next;
}t_rifornimento;



void rifornimento(t_magazzino* magazzino, int tempo, char* comando ) {
    char* token = strtok(comando, " ");
    token = strtok(NULL, " ");

    t_rifornimento* head = NULL;
    t_rifornimento* curr = NULL;

    while (token != NULL) {
        char* nome_ingrediente = token;
        int quantita = atoi(strtok(NULL, " "));
        int scadenza = atoi(strtok(NULL, " "));

        t_rifornimento* nuovo = malloc(sizeof(t_rifornimento));
        strcpy(nuovo->nome_ingrediente,nome_ingrediente);
        nuovo->quantita= quantita;
        nuovo->scadenza=scadenza;

        if (head == NULL) {
            
            head = nuovo;
            curr = head;
        } else {
            curr->next = nuovo;
            curr = nuovo;
        }


        aggiungi_lotto(magazzino, nome_ingrediente, quantita, scadenza);

        token = strtok(NULL, " ");

        
        
    }

    curr->next=NULL;

    
    
    printf("rifornito\n");


}



typedef struct ordine{
    int tempo_arrivo;
    char nome_ricetta[MAX_NOME];
    int quantita;
    int preparato;
    int peso_totale;
    struct ordine* next;
} t_ordine;

typedef struct {
    t_ordine* head;
    t_ordine* tail;
    int num_ordini;
    int peso_totale;
} t_coda_ordini;

t_coda_ordini* crea_coda_ordini() {
    t_coda_ordini* coda = malloc(sizeof(t_coda_ordini));
    coda->num_ordini = 0;
    coda->head  = coda->tail = NULL;
    
    coda->peso_totale = 0;
    return coda;
}

t_ordine* aggiungi_ordine(t_coda_ordini* coda, int tempo_arrivo, const char* nome_ricetta, int quantita, int peso_totale) {
    t_ordine* nuovo = malloc(sizeof(t_ordine));
    nuovo->tempo_arrivo = tempo_arrivo;
    strcpy(nuovo->nome_ricetta, nome_ricetta);
    nuovo->quantita = quantita;
    nuovo->preparato = 0;
    nuovo->peso_totale = peso_totale;
    nuovo->next = NULL;

    if (coda->tail == NULL) {
        coda->head = coda->tail = nuovo;
    } else {
        coda->tail->next = nuovo;
        coda->tail = nuovo;
    }

    coda->num_ordini++;
    coda->peso_totale += peso_totale;

    return nuovo;
}

int prepara_ordine(t_ordine* ordine, t_magazzino* magazzino, t_ricetta* ricetta, int tempo) {

    t_ingr_magazzino* ingredienti[ricetta->num_ingr];

    for (int i = 0; i < ricetta->num_ingr; i++) {
        int richiesti = ricetta->ingredienti[i].quantita * ordine->quantita;
        ingredienti[i]= cerca_ingr_magazzino(magazzino, ricetta->ingredienti[i].nome);

        if(ingredienti[i] == NULL || ingredienti[i]->num_lotti<richiesti){
            return 0 ;
        }

    }
    
    
    int peso_totale = 0;
    for (int i = 0; i < ricetta->num_ingr; i++) {
        int richiesti = ricetta->ingredienti[i].quantita * ordine->quantita;
        usa_ingrediente(ingredienti[i], ricetta->ingredienti[i].nome, richiesti, tempo);
        peso_totale += richiesti;

    }
    
    ordine->preparato = 1;
    ordine->peso_totale = peso_totale;   
    return 1;
}

void processa_ordine(t_coda_ordini* coda, t_ricettario* ricettario, t_magazzino* magazzino, int tempo, const char* nome_ricetta, int quantita) {
    
    t_ricetta* ricetta = cerca_ricetta(ricettario, nome_ricetta);
    
    if (ricetta==NULL) {
        printf("rifiutato\n");
        return;
    }
    
    t_ordine* nuovo = malloc(sizeof(t_ordine));
    nuovo->tempo_arrivo = tempo;
    strcpy(nuovo->nome_ricetta, nome_ricetta);
    nuovo->quantita = quantita;
    nuovo->preparato = 0;
    nuovo->peso_totale=0;
    nuovo->next = NULL;

    if(coda->tail == NULL){
        coda->head = coda->tail = nuovo;
    }else{
        coda->tail->next = nuovo;
        coda->tail = nuovo;
    }
    coda->num_ordini++;

    prepara_ordine(nuovo, magazzino, ricetta, tempo);
    printf("accettato\n");

}

void processa_ordini_in_attesa(t_coda_ordini* coda, t_magazzino* magazzino, t_ricettario* ricettario, int tempo) {

    t_ordine* curr = coda->head;

    t_ricetta* ricetta = NULL;

    

    while(curr !=NULL){
        if(!curr->preparato){

            ricetta = cerca_ricetta(ricettario, curr->nome_ricetta);
            prepara_ordine( curr, magazzino,ricetta, tempo);
        }

        curr=curr->next;
        
    }


}

typedef struct {
    int capienza;
    int periodicita;
    int carico_corrente;
    t_ordine** ordini_caricati;
    int num_ordini_caricati;
    int max_ordini_caricabili;
} t_corriere;

t_corriere* crea_corriere(int capienza, int periodicita) {
    t_corriere* corriere = malloc(sizeof(t_corriere));
    corriere->capienza = capienza;
    corriere->periodicita = periodicita;
    corriere->max_ordini_caricabili = 400;
    corriere->carico_corrente = 0;
    corriere->ordini_caricati = malloc(sizeof(t_ordine*) * corriere->max_ordini_caricabili);
    corriere->num_ordini_caricati = 0;
    return corriere;
}

void swap(t_ordine** a, t_ordine** b) {
    t_ordine* t = *a;
    *a = *b;
    *b = t;
}

int partition(t_ordine** arr, int low, int high) {
    t_ordine* pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->peso_totale > pivot->peso_totale || (arr[j]->peso_totale == pivot->peso_totale && arr[j]->tempo_arrivo < pivot->tempo_arrivo)) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}


void quicksort(t_ordine** arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}





void carica_corriere(t_corriere* corriere, t_coda_ordini* coda) {
    corriere->carico_corrente = 0;
    corriere->num_ordini_caricati = 0;

    t_ordine* current = coda->head;
    t_ordine* prev = NULL;

    while (current != NULL) {
        if (current->preparato) {
            if(corriere->carico_corrente + current->peso_totale > corriere->capienza){
                break;
            }
            if (prev == NULL) {
                coda->head = current->next;
            } else {
                prev->next = current->next;
            }
            if (coda->tail == current) {
                coda->tail = prev;
            }

            if (corriere->num_ordini_caricati == corriere->max_ordini_caricabili) {
                corriere->max_ordini_caricabili *= 2;
                corriere->ordini_caricati = realloc(corriere->ordini_caricati, sizeof(t_ordine*) * corriere->max_ordini_caricabili);
            }
            corriere->ordini_caricati[corriere->num_ordini_caricati++] = current;
            corriere->carico_corrente += current->peso_totale;

            coda->num_ordini--;

            if (prev == NULL) {
                current = coda->head;
            } else {
                current = prev->next;
            }
        } else {
            prev = current;
            current = current->next;
        }
    }

    if (corriere->num_ordini_caricati > 1) {
        quicksort(corriere->ordini_caricati, 0, corriere->num_ordini_caricati - 1);
    }
}

void stampa_carico_corriere(t_corriere* corriere) {
    if (corriere->num_ordini_caricati == 0) {
        printf("camioncino vuoto\n");
    } else {
        for (int i = 0; i < corriere->num_ordini_caricati; i++) {
            t_ordine* ordine = corriere->ordini_caricati[i];
            printf("%d %s %d\n", ordine->tempo_arrivo, ordine->nome_ricetta, ordine->quantita);
            free(ordine);
        }
    }
}


void processa_aggiungi_ricetta(t_ricettario* ricettario, char* comando) {
    char* token = strtok(comando, " ");
    token = strtok(NULL, " ");  
    
    t_ricetta nuova_ricetta;
    strcpy(nuova_ricetta.nome, token);
    nuova_ricetta.num_ingr = 0;
    nuova_ricetta.peso = 0;
    
    while ((token = strtok(NULL, " ")) != NULL) {
        
        strcpy(nuova_ricetta.ingredienti[nuova_ricetta.num_ingr].nome, token);
        
        token = strtok(NULL, " ");
        int quantita = atoi(token);
        nuova_ricetta.ingredienti[nuova_ricetta.num_ingr].quantita = quantita;
        nuova_ricetta.peso += quantita;
        nuova_ricetta.num_ingr++;
    }
    
    aggiungi_ricetta(ricettario, &nuova_ricetta);
}

void processa_rimuovi_ricetta(t_ricettario* ricettario, t_coda_ordini* coda, char* nome_ricetta) {

    t_ordine * curr = coda->head;
    for (int i = 0; i < coda->num_ordini; i++) {
        if (strcmp(curr->nome_ricetta, nome_ricetta) == 0) {
            printf("ordini in sospeso\n");
            return;
        }
        curr = curr->next;
    }
    
    rimuovi_ricetta(ricettario, nome_ricetta);
}



void free_magazzino(t_magazzino* magazzino) {
    for (int i = 0; i < DIM_HASH_TABLE2; i++) {
        t_ingr_magazzino* ingr = magazzino->tabella[i];
        while (ingr != NULL) {
            t_lotto* curr = ingr->lotti;
            while (curr != NULL) {
                t_lotto* temp = curr;
                curr = curr->next;
                free(temp);
            }
            t_ingr_magazzino* temp = ingr;
            ingr = ingr->next;
            free(temp);
        }
    }
    free(magazzino);
}

void free_corriere(t_corriere* corriere) {

    free(corriere->ordini_caricati);
    free(corriere);
}

void free_coda(t_coda_ordini* coda) {
    while (coda->head != NULL) {
        t_ordine* temp = coda->head;
        coda->head = coda->head->next;
        free(temp);
    }
    free(coda);
}



int main() {
    
    t_ricettario* ricettario = crea_ricettario();
    t_magazzino* magazzino = crea_magazzino();
    t_coda_ordini* coda_ordini = crea_coda_ordini();
    t_corriere* corriere;
    int tempo = 0;

    int periodicita, capienza;

    char comando[5000];

    if(fgets(comando, sizeof(comando), stdin)==NULL){
        fprintf(stderr, "Error reading corriere configuration\n");
        exit(1);
    }
    
    periodicita = atoi(strtok(comando, " "));
    capienza = atoi(strtok(NULL, " ")); 

    corriere = crea_corriere(capienza, periodicita);



    while (fgets(comando, sizeof(comando), stdin)) {

        comando[strcspn(comando, "\n")] = 0; 

        if (strncmp(comando, "aggiungi_ricetta", 16) == 0) {
            processa_aggiungi_ricetta(ricettario, comando);
        } else if (strncmp(comando, "rimuovi_ricetta", 15) == 0) {
            char nome_ricetta[MAX_NOME];
            sscanf(comando, "rimuovi_ricetta %s", nome_ricetta);
            processa_rimuovi_ricetta(ricettario, coda_ordini, nome_ricetta);
        } else if (strncmp(comando, "rifornimento", 12) == 0) {
            rifornimento(magazzino, tempo, comando);

            rimuovi_lotti_scaduti(magazzino, tempo);
            
            processa_ordini_in_attesa(coda_ordini, magazzino, ricettario, tempo);
        } else if (strncmp(comando, "ordine", 6) == 0) {
            char nome_ricetta[MAX_NOME];
            int quantita;
            if (sscanf(comando, "ordine %s %d", nome_ricetta, &quantita) == 2) {
                processa_ordine(coda_ordini, ricettario, magazzino, tempo, nome_ricetta, quantita);
            }
        }

        tempo++;

        if (tempo % periodicita == 0) {
            carica_corriere(corriere, coda_ordini);
            stampa_carico_corriere(corriere);
        }



    }

    return 0;
}