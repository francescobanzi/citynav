#include "map.h"

// Crea una nuova mappa vuota
Map* create_map() {
    Map* m = (Map*)malloc(sizeof(Map));
    m->num_nodes = 0;
    m->next_id = 1;  // Iniziamo da 1 per ID significativi
    
    for (int i = 0; i < MAX_NODES; i++) {
        m->adj_list[i] = NULL;
        m->nodes[i].id = -1;
    }
    
    return m;
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
    //add_road(m, 3, 1, 2000);  // Ospedale -> Stazione
    add_road(m, 4, 5, 600);   // Universita -> Mercato
    add_road(m, 6, 4, 700);   // Parco -> Universita
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

// Funzione per aggiungere un POI
void interactive_add_poi(Map *m) {
    char name[MAX_NAME];
    char category[MAX_CATEGORY];
    
    printf("\n--- AGGIUNTA NUOVO POI ---\n");
    
    printf("Inserisci il nome del POI: ");
    clear_input_buffer();
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("Inserisci la categoria: ");
    fgets(category, MAX_CATEGORY, stdin);
    category[strcspn(category, "\n")] = 0;
    
    add_poi(m, name, category);
}

// Funzione per aggiungere una strada interattivamente
void interactive_add_road(Map *m) {
    int from, to;
    float weight;
    
    if (m->num_nodes < 2) {
        printf("Errore: Servono almeno 2 POI!\n");
        return;
    }
    
    printf("\n--- AGGIUNTA NUOVA STRADA ---\n");
    list_all_pois(m);
    
    printf("Inserisci ID partenza: ");
    scanf("%d", &from);
    
    if (!node_exists(m, from)) return;
    
    printf("Inserisci ID destinazione: ");
    scanf("%d", &to);
    
    if (!node_exists(m, to)) return;
    
    printf("Distanza: ");
    scanf("%f", &weight);
    
    if (weight <= 0) return;
    
    add_road(m, from, to, weight);
}

// Funzione per calcolare percorso
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
}

// Funzione per rimuovere un POI
void interactive_remove_poi(Map *m) {
    int id;
    
    if (m->num_nodes == 0) return;  // niente da fare
    
    list_all_pois(m);
    printf("Inserisci l'ID del POI da rimuovere: ");
    scanf("%d", &id);
    
    if (!node_exists(m, id)) return; // nodo inesistente
    
    remove_poi(m, id);
}


// Funzione per bloccare una strada interattivamente
void interactive_block_road(Map *m) {
    int from, to;

    if (m->num_nodes < 2) {
        printf("Errore: Servono almeno 2 POI per bloccare una strada!\n");
        return;
    }

    printf("\n--- BLOCCO STRADA ---\n");
    list_all_pois(m);

    printf("Inserisci l'ID del nodo di partenza della strada da bloccare: ");
    scanf("%d", &from);

    if (!node_exists(m, from)) {
        printf("Errore: Il nodo %d non esiste!\n", from);
        return;
    }

    printf("Inserisci l'ID del nodo di destinazione della strada da bloccare: ");
    scanf("%d", &to);

    if (!node_exists(m, to)) {
        printf("Errore: Il nodo %d non esiste!\n", to);
        return;
    }

    block_road(m, from, to);
}

// Funzione per testare i sensi unici
void test_one_way_streets(Map *m) {
    printf("\n=== TEST SENSI UNICI ===\n");
    
    printf("Test 1:\n");
    shortest_path(m, 1, 2);
    
    printf("Test 2:\n");
    shortest_path(m, 2, 1);
}

// F1 - Aggiunge un nuovo punto di interesse
void add_poi(Map *m, char *name, char *category) {
    if (m->num_nodes >= MAX_NODES) {
        printf("Errore: numero massimo di nodi raggiunto\n");
        return;
    }
    
    int id = m->next_id++;
    m->nodes[id].id = id;
    strcpy(m->nodes[id].name, name);
    strcpy(m->nodes[id].category, category);
    m->num_nodes++;
    
    printf("POI aggiunto: ID=%d, Nome=%s, Categoria=%s\n", id, name, category);
}

// F2 - Aggiunge una strada orientata
void add_road(Map *m, int from, int to, float weight) {
    if (!node_exists(m, from) || !node_exists(m, to)) {
        printf("Errore: nodo inesistente (from=%d, to=%d)\n", from, to);
        return;
    }
    if (from == to) {
    printf("Errore: non puoi collegare un nodo a se stesso!\n");
    return;
}
    if (weight <= 0) {
        printf("Errore: il peso deve essere positivo\n");
        return;
    }
    
    // Crea nuovo arco
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    new_edge->from = from;
    new_edge->to = to;
    new_edge->weight = weight;
    new_edge->next = m->adj_list[from];
    m->adj_list[from] = new_edge;
    
    printf("Strada aggiunta: %d -> %d (peso: %.2f)\n", from, to, weight);
}

// Implementazione dell'algoritmo di Dijkstra per il percorso minimo
void shortest_path(Map *m, int from, int to) {
    if (!node_exists(m, from) || !node_exists(m, to)) {
        printf("Errore: nodi inesistenti (from=%d, to=%d)\n", from, to);
        return;
    }
    
    float dist[MAX_NODES];
    int prev[MAX_NODES];
    bool visited[MAX_NODES];
    
    // Inizializzazione
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }
    dist[from] = 0;
    
    // Algoritmo di Dijkstra
    for (int count = 0; count < m->num_nodes; count++) {
        // Trova il nodo non visitato con distanza minima
        float min_dist = INF;
        int min_node = -1;
        
        for (int i = 0; i < MAX_NODES; i++) {
            if (m->nodes[i].id != -1 && !visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = true;
        
        // Aggiorna le distanze dei vicini
        Edge* edge = m->adj_list[min_node];
        while (edge != NULL) {
            if (!visited[edge->to]) {
                float new_dist = dist[min_node] + edge->weight;
                if (new_dist < dist[edge->to]) {
                    dist[edge->to] = new_dist;
                    prev[edge->to] = min_node;
                }
            }
            edge = edge->next;
        }
    }
    
    // Stampa il risultato
    if (dist[to] == INF) {
        printf("Nessun percorso trovato da %d a %d\n", from, to);
        return;
    }
    
    // Ricostruisci il percorso
    int path[MAX_NODES];
    int path_len = 0;
    int current = to;
    
    while (current != -1) {
        path[path_len++] = current;
        current = prev[current];
    }
    
    // Stampa il percorso
    printf("\n=== PERCORSO TROVATO ===\n");
    printf("Da: %s (ID %d)\n", m->nodes[from].name, from);
    printf("A: %s (ID %d)\n", m->nodes[to].name, to);
    printf("Costo totale: %.2f\n", dist[to]);
    printf("Percorso: ");
    
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%s", m->nodes[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n\n");
}

// F4 - Rimuove un punto di interesse
void remove_poi(Map *m, int id) {
    if (!node_exists(m, id)) {
        printf("Errore: nodo %d inesistente\n", id);
        return;
    }
    
    // Rimuovi tutti gli archi in uscita dal nodo
    remove_edges_from_node(m, id);
    
    // Rimuovi tutti gli archi in entrata al nodo
    for (int i = 0; i < MAX_NODES; i++) {
        if (m->nodes[i].id != -1 && i != id) {
            Edge* prev = NULL;
            Edge* curr = m->adj_list[i];
            
            while (curr != NULL) {
                if (curr->to == id) {
                    if (prev == NULL) {
                        m->adj_list[i] = curr->next;
                        free(curr);
                        curr = m->adj_list[i];
                    } else {
                        prev->next = curr->next;
                        free(curr);
                        curr = prev->next;
                    }
                } else {
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
    }
    
    // Rimuovi il nodo
    printf("Rimosso POI: %s (ID %d)\n", m->nodes[id].name, id);
    m->nodes[id].id = -1;
    m->num_nodes--;
}

// Funzioni di utilità
bool node_exists(Map *m, int id) {
    return (id >= 0 && id < MAX_NODES && m->nodes[id].id != -1);
}

void remove_edges_from_node(Map *m, int id) {
    Edge* curr = m->adj_list[id];
    while (curr != NULL) {
        Edge* temp = curr;
        curr = curr->next;
        free(temp);
    }
    m->adj_list[id] = NULL;
}

int find_poi_by_name(Map *m, char *name) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (m->nodes[i].id != -1 && strcmp(m->nodes[i].name, name) == 0) {
            return m->nodes[i].id;
        }
    }
    return -1;
}

void print_poi(Map *m, int id) {
    if (node_exists(m, id)) {
        printf("ID: %d, Nome: %s, Categoria: %s\n", 
               id, m->nodes[id].name, m->nodes[id].category);
    }
}

void print_map(Map *m) {
    printf("\n=== MAPPA CORRENTE ===\n");
    printf("Nodi totali: %d\n", m->num_nodes);
    
    printf("\nPunti di Interesse:\n");
    for (int i = 0; i < MAX_NODES; i++) {
        if (m->nodes[i].id != -1) {
            printf("  %d: %s [%s]\n", i, m->nodes[i].name, m->nodes[i].category);
        }
    }
    
    printf("\nStrade (orientate):\n");
    for (int i = 0; i < MAX_NODES; i++) {
        if (m->nodes[i].id != -1) {
            Edge* edge = m->adj_list[i];
            while (edge != NULL) {
                printf("  %d (%s) -> %d (%s) [%.2f]\n", 
                       edge->from, m->nodes[edge->from].name,
                       edge->to, m->nodes[edge->to].name,
                       edge->weight);
                edge = edge->next;
            }
        }
    }
    printf("========================\n\n");
}

// Blocca (rimuove) un arco orientato tra due nodi
void block_road(Map *m, int from, int to) {
    if (!node_exists(m, from) || !node_exists(m, to)) {
        printf("Errore: nodo inesistente (from=%d, to=%d)\n", from, to);
        return;
    }

    Edge *prev = NULL;
    Edge *curr = m->adj_list[from];

    while (curr != NULL) {
        if (curr->to == to) {
            if (prev == NULL) {
                m->adj_list[from] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Strada bloccata: %d (%s) -> %d (%s)\n",
                   from, m->nodes[from].name,
                   to,   m->nodes[to].name);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("Errore: nessuna strada trovata da %d (%s) a %d (%s)\n",
           from, m->nodes[from].name,
           to,   m->nodes[to].name);
}

void free_map(Map *m) {
    for (int i = 0; i < MAX_NODES; i++) {
        remove_edges_from_node(m, i);
    }
    free(m);
}