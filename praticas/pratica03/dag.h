#ifndef DAG_H
#define DAG_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    int dirigido;
    No **adj; 
} GrafoLista;

GrafoLista *grafo_criar(int num_vertices, int dirigido);
void grafo_destruir(GrafoLista *g);
void grafo_add_aresta(GrafoLista *g, int origem, int destino);
void grafo_imprimir(const GrafoLista *g);

int eh_dag(GrafoLista *g);


int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);

#endif