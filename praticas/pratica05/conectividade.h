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

#endif
