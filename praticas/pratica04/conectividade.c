#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

GrafoLista *criar_grafo(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    if (!g) return NULL;

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
    a->destino = v;
    a->prox = g->adj[u];
    g->adj[u] = a;

    a = malloc(sizeof(Aresta));
    a->destino = u;
    a->prox = g->adj[v];
    g->adj[v] = a;
}

void liberar_grafo(GrafoLista *g) {
    if (!g) return;

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

void dfs_articulacoes(GrafoLista *g, int u, int pai, int *tempo,
                      int *descoberta, int *low, int *articulacao) {
    descoberta[u] = low[u] = ++(*tempo);
    int filhos = 0;

    for (Aresta *a = g->adj[u]; a; a = a->prox) {
        int v = a->destino;

        if (descoberta[v] == 0) {
            filhos++;

            dfs_articulacoes(g, v, u, tempo, descoberta, low, articulacao);

            if (low[v] < low[u])
                low[u] = low[v];

            if (pai == -1 && filhos > 1)
                articulacao[u] = 1;

            if (pai != -1 && low[v] >= descoberta[u])
                articulacao[u] = 1;
        } else if (v != pai && descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }
}

void detectar_articulacoes(GrafoLista *g, int *articulacao) {
    int *descoberta = calloc(g->n, sizeof(int));
    int *low = calloc(g->n, sizeof(int));
    int tempo = 0;

    for (int i = 0; i < g->n; i++)
        articulacao[i] = 0;

    for (int i = 0; i < g->n; i++) {
        if (descoberta[i] == 0)
            dfs_articulacoes(g, i, -1, &tempo, descoberta, low, articulacao);
    }

    free(descoberta);
    free(low);
}

static void dfs_pontes(GrafoLista *g, int u, int pai, int *tempo,
                       int *descoberta, int *low) {
    descoberta[u] = low[u] = ++(*tempo);

    for (Aresta *a = g->adj[u]; a; a = a->prox) {
        int v = a->destino;

        if (descoberta[v] == 0) {
            dfs_pontes(g, v, u, tempo, descoberta, low);

            if (low[v] < low[u])
                low[u] = low[v];

            if (low[v] > descoberta[u])
                printf("(%d, %d)\n", u, v);
        } else if (v != pai && descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }
}

void detectar_pontes(GrafoLista *g) {
    int *descoberta = calloc(g->n, sizeof(int));
    int *low = calloc(g->n, sizeof(int));
    int tempo = 0;

    for (int i = 0; i < g->n; i++) {
        if (descoberta[i] == 0)
            dfs_pontes(g, i, -1, &tempo, descoberta, low);
    }

    free(descoberta);
    free(low);
}
