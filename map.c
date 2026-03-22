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

void free_map(Map *m) {
    for (int i = 0; i < MAX_NODES; i++) {
        remove_edges_from_node(m, i);
    }
    free(m);
}