#include <stdlib.h>
#include "conectividade.h"

GrafoLista *criar_grafo(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));

    if (!g)
        return NULL;

    g->n = n;
    g->adj = calloc(n, sizeof(Aresta *));

    if (!g->adj) {
        free(g);
        return NULL;
    }

    return g;
}

void adicionar_aresta(GrafoLista *g, int u, int v) {
    if (!g || u < 0 || u >= g->n || v < 0 || v >= g->n || u == v)
        return;

    Aresta *a = malloc(sizeof(Aresta));

    if (!a)
        return;

    a->destino = v;
    a->prox = g->adj[u];
    g->adj[u] = a;

    a = malloc(sizeof(Aresta));

    if (!a)
        return;

    a->destino = u;
    a->prox = g->adj[v];
    g->adj[v] = a;
}

void liberar_grafo(GrafoLista *g) {
    if (!g)
        return;

    for (int i = 0; i < g->n; i++) {
        Aresta *a = g->adj[i];

        while (a) {
            Aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }

    free(g->adj);
    free(g);
}
