#include "map.h"
#include <ctype.h>

// Funzione per pulire il buffer di input
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funzione per stampare il menu
void print_menu() {
    printf("\n========================================\n");
    printf("   C I T Y N A V   -   M E N U   \n");
    printf("========================================\n");
    printf("1. Aggiungi un POI (Punto di Interesse)\n");
    printf("2. Aggiungi una strada\n");
    printf("3. Calcola percorso minimo\n");
    printf("4. Rimuovi un POI\n");
    printf("5. Visualizza mappa completa\n");
    printf("0. Esci\n");
    printf("========================================\n");
    printf("Scegli un'opzione: ");
}

// Funzione per mostrare tutti i POI disponibili
void list_all_pois(Map *m) {
    printf("\n--- PUNTI DI INTERESSE DISPONIBILI ---\n");
    int count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        if (m->nodes[i].id != -1) {
            printf("ID: %-3d | Nome: %-25s | Categoria: %s\n", 
                   i, m->nodes[i].name, m->nodes[i].category);
            count++;
        }
    }
    if (count == 0) {
        printf("Nessun POI presente nella mappa.\n");
    }
    printf("--------------------------------------\n");
}

// Funzione per aggiungere un POI interattivamente
void interactive_add_poi(Map *m) {
    char name[MAX_NAME];
    char category[MAX_CATEGORY];
    
    printf("\n--- AGGIUNTA NUOVO POI ---\n");
    
    printf("Inserisci il nome del POI: ");
    clear_input_buffer();
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;  // Rimuovi newline
    
    printf("Inserisci la categoria (es. trasporti, cultura, ospedali, ristorazione, verde): ");
    fgets(category, MAX_CATEGORY, stdin);
    category[strcspn(category, "\n")] = 0;
    
    add_poi(m, name, category);
}

// Funzione per aggiungere una strada interattivamente
void interactive_add_road(Map *m) {
    int from, to;
    float weight;
    
    if (m->num_nodes < 2) {
        printf("Errore: Servono almeno 2 POI per creare una strada!\n");
        return;
    }
    
    printf("\n--- AGGIUNTA NUOVA STRADA ---\n");
    list_all_pois(m);
    
    printf("Inserisci l'ID del nodo di partenza: ");
    scanf("%d", &from);
    
    if (!node_exists(m, from)) {
        printf("Errore: Il nodo %d non esiste!\n", from);
        return;
    }
    
    printf("Inserisci l'ID del nodo di destinazione: ");
    scanf("%d", &to);
    
    if (!node_exists(m, to)) {
        printf("Errore: Il nodo %d non esiste!\n", to);
        return;
    }
    
    printf("Inserisci la distanza (in metri): ");
    scanf("%f", &weight);
    
    if (weight <= 0) {
        printf("Errore: La distanza deve essere positiva!\n");
        return;
    }
    
    add_road(m, from, to, weight);
}

// Funzione per calcolare percorso interattivamente
void interactive_shortest_path(Map *m) {
    int from, to;
    
    if (m->num_nodes < 2) {
        printf("Errore: Servono almeno 2 POI per calcolare un percorso!\n");
        return;
    }
    
    printf("\n--- CALCOLO PERCORSO MINIMO ---\n");
    list_all_pois(m);
    
    printf("Inserisci l'ID del punto di partenza: ");
    scanf("%d", &from);
    
    if (!node_exists(m, from)) {
        printf("Errore: Il nodo %d non esiste!\n", from);
        return;
    }
    
    printf("Inserisci l'ID della destinazione: ");
    scanf("%d", &to);
    
    if (!node_exists(m, to)) {
        printf("Errore: Il nodo %d non esiste!\n", to);
        return;
    }
    
    printf("\nCalcolo percorso in corso...\n");
    shortest_path(m, from, to);
    
    // Messaggio informativo sul grafo orientato
    printf("\nNota: Il grafo è orientato (sensi unici).\n");
    printf("Il percorso inverso potrebbe non esistere!\n");
}

// Funzione per rimuovere un POI interattivamente
void interactive_remove_poi(Map *m) {
    int id;
    
    if (m->num_nodes == 0) {
        printf("Errore: Nessun POI da rimuovere!\n");
        return;
    }
    
    printf("\n--- RIMOZIONE POI ---\n");
    list_all_pois(m);
    
    printf("Inserisci l'ID del POI da rimuovere: ");
    scanf("%d", &id);
    
    if (!node_exists(m, id)) {
        printf("Errore: Il nodo %d non esiste!\n", id);
        return;
    }
    
    printf("Sei sicuro di voler rimuovere '%s'? (s/n): ", m->nodes[id].name);
    clear_input_buffer();
    char confirm = getchar();
    
    if (tolower(confirm) == 's') {
        remove_poi(m, id);
        printf("POI rimosso con successo!\n");
    } else {
        printf("Operazione annullata.\n");
    }
}

// Crea la mappa di esempio di Bologna
void create_sample_map(Map *m) {
    printf("\n=== CREAZIONE MAPPA DI BOLOGNA ===\n");
    
    // Aggiunta POI
    add_poi(m, "Stazione AV", "trasporti");        
    add_poi(m, "Piazza Maggiore", "cultura");      
    add_poi(m, "Ospedale Sant'Orsola", "ospedali"); 
    add_poi(m, "Universita", "cultura");           
    add_poi(m, "Mercato delle Erbe", "ristorazione"); 
    add_poi(m, "Parco della Montagnola", "verde"); 
    
    // Aggiunta strade 
    printf("\n=== AGGIUNTA STRADE ===\n");
    add_road(m, 1, 2, 800);   // Stazione -> Piazza Maggiore
    add_road(m, 2, 4, 500);   // Piazza Maggiore -> Universita
    add_road(m, 4, 3, 1200);  // Universita -> Ospedale
    add_road(m, 1, 6, 600);   // Stazione -> Parco
    add_road(m, 6, 5, 400);   // Parco -> Mercato
    add_road(m, 5, 2, 300);   // Mercato -> Piazza Maggiore
    add_road(m, 3, 1, 2000);  // Ospedale -> Stazione
    add_road(m, 4, 5, 600);   // Universita -> Mercato
    add_road(m, 6, 4, 700);   // Parco -> Universita
    
    printf("\nMappa di esempio caricata con successo!\n");
    printf("\n6 POI, 9 strade orientate\n");
}

// Funzione per testare i sensi unici (!!!da aggiungere al menù!!!)
void test_one_way_streets(Map *m) {
    printf("\n=== TEST SENSI UNICI ===\n");
    printf("Questo test dimostra che il grafo è orientato:\n\n");
    
    printf("Test 1: Stazione (1) -> Piazza Maggiore (2):\n");
    shortest_path(m, 1, 2);
    
    printf("Test 2: Piazza Maggiore (2) -> Stazione (1) (dovrebbe fallire):\n");
    shortest_path(m, 2, 1);
}

int main() {
    Map *city_map = create_map(); //DA FIXARE
    int choice; //AGGIUNGERE CONTROLLO DELL'INPUT 
       
    printf("========================================\n");
    printf("   C I T Y N A V   -   B O L O G N A   \n");
    printf("   Sistema di Navigazione Urbana\n");
    printf("========================================\n");
    
    // Carica automaticamente la mappa di esempio all'avvio
    printf("\nCaricamento mappa di esempio...\n");
    create_sample_map(city_map);
    
    // Mostra la mappa iniziale
    printf("\n--- MAPPA INIZIALE ---\n");
    print_map(city_map);
    
    // Menu interattivo
    do {
        print_menu();
        
        // Gestione input numerico
        if (scanf("%d", &choice) != 1) {
            printf("Errore: inserisci un numero!\n");
            clear_input_buffer();
            continue;
        }

        if (choice < 0 || choice > 5) {
            printf("Errore: numero tra 0 e 5!\n");
            continue;
        }
        
        switch(choice) {
            case 1:
                interactive_add_poi(city_map);
                break;
                
            case 2:
                interactive_add_road(city_map);
                break;
                
            case 3:
                interactive_shortest_path(city_map);
                break;
                
            case 4:
                interactive_remove_poi(city_map);
                break;
                
            case 5:
                printf("\n--- VISUALIZZAZIONE MAPPA COMPLETA ---\n");
                print_map(city_map);
                break;
                
            case 8:  // Opzione nascosta per test sensi unici
                test_one_way_streets(city_map);
                break;
                
            case 0:
                printf("\nGrazie per aver usato CityNav! Arrivederci!\n");
                break;
                
            default:
                printf("\nOpzione non valida. Riprova.\n");
        }
        
    } while (choice != 0);
    
    // Pulizia memoria
    free_map(city_map);
    
    return 0;
}