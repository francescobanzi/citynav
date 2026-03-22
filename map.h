#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

#define MAX_NAME 50
#define MAX_CATEGORY 30
#define MAX_NODES 100
#define INF FLT_MAX

// Struttura per un punto di interesse (nodo)
typedef struct {
    int id;
    char name[MAX_NAME];
    char category[MAX_CATEGORY];
} POI;

// Struttura per un arco del grafo (strada)
typedef struct Edge {
    int from;
    int to;
    float weight;
    struct Edge* next;
} Edge;

// Struttura principale della mappa (grafo orientato pesato)
typedef struct {
    POI nodes[MAX_NODES];
    Edge* adj_list[MAX_NODES];
    int num_nodes;
    int next_id;
} Map;

// Funzioni obbligatorie
Map* create_map();
void add_poi(Map *m, char *name, char *category);
void add_road(Map *m, int from, int to, float weight);
void shortest_path(Map *m, int from, int to);
void remove_poi(Map *m, int id);
void free_map(Map *m);

// Funzioni di utilità
int find_poi_by_name(Map *m, char *name);
void print_poi(Map *m, int id);
void print_map(Map *m);
bool node_exists(Map *m, int id);
void remove_edges_from_node(Map *m, int id);
