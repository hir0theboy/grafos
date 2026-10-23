#include <stdlib.h>
#include "grafo_matriz.h"

GrafoMatriz *criar_grafo_matriz(int n) {
    GrafoMatriz *grafo;

    if (n <= 0) {
        return NULL;
    }

    grafo = malloc(sizeof(GrafoMatriz));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->n = n;

    grafo->adj = malloc(n * sizeof(int *));

    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        grafo->adj[i] = calloc(n, sizeof(int));

        if (grafo->adj[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(grafo->adj[j]);
            }

            free(grafo->adj);
            free(grafo);

            return NULL;
        }
    }

    return grafo;
}

void inserir_aresta_matriz(GrafoMatriz *grafo, int u, int v) {
    if (grafo == NULL) {
        return;
    }

    if (u < 0 || u >= grafo->n ||
        v < 0 || v >= grafo->n) {
        return;
    }

    if (u == v) {
        return;
    }

    grafo->adj[u][v] = 1;
    grafo->adj[v][u] = 1;
}

void remover_aresta_matriz(GrafoMatriz *grafo, int u, int v) {
    if (grafo == NULL) {
        return;
    }

    if (u < 0 || u >= grafo->n ||
        v < 0 || v >= grafo->n) {
        return;
    }

    grafo->adj[u][v] = 0;
    grafo->adj[v][u] = 0;
}

int grau_matriz(GrafoMatriz *grafo, int vertice) {
    if (grafo == NULL) {
        return -1;
    }

    if (vertice < 0 || vertice >= grafo->n) {
        return -1;
    }

    int grau = 0;

    for (int i = 0; i < grafo->n; i++) {
        grau += grafo->adj[vertice][i];
    }

    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *grafo, int u, int v) {
    if (grafo == NULL) {
        return 0;
    }

    if (u < 0 || u >= grafo->n ||
        v < 0 || v >= grafo->n) {
        return 0;
    }

    return grafo->adj[u][v];
}

void liberar_grafo_matriz(GrafoMatriz *grafo) {
    if (grafo == NULL) {
        return;
    }

    for (int i = 0; i < grafo->n; i++) {
        free(grafo->adj[i]);
    }

    free(grafo->adj);
    free(grafo);
}