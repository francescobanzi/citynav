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
    printf("6. Blocca una strada\n");
    printf("7. Esegui test percorrenza (uno di successo, uno di fallimento)\n");
    printf("0. Esci\n");
    printf("========================================\n");
    printf("Scegli un'opzione: ");
}

int main() {
    Map *city_map = create_map();
    int choice;
       
    printf("========================================\n");
    printf("   C I T Y N A V   -   B O L O G N A   \n");
    printf("   Sistema di Navigazione Urbana\n");
    printf("========================================\n");
    
    // Carica automaticamente la mappa di esempio all'avvio
    printf("\nCaricamento mappa di esempio...\n");
    create_sample_map(city_map);
    
    // Menu interattivo
    do {
        print_menu();
        
        // Gestione input numerico
        if (scanf("%d", &choice) != 1) {
            printf("Errore: inserisci un numero!\n");
            clear_input_buffer();
            continue;
        }

        if (choice < 0 || choice > 7) {
            printf("Errore: numero tra 0 e 7!\n");
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

            case 6:
                interactive_block_road(city_map);
                break;
                
            case 7:
                printf("\n--- TEST SENSI UNICI ---\n");
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