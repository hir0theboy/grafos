#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

typedef struct Aresta {
    int destino;
    struct Aresta *prox;
} Aresta;

typedef struct {
    int n;
    Aresta **adj;
} GrafoLista;

GrafoLista *criar_grafo(int n);
void adicionar_aresta(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);
void dfs_articulacoes(GrafoLista *g, int u, int pai, int *tempo, int *descoberta, int *low, int *articulacao);
void detectar_articulacoes(GrafoLista *g, int *articulacao);
void detectar_pontes(GrafoLista *g);

#endif
