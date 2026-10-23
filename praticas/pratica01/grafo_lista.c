#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *grafo;

    if (n <= 0) {
        return NULL;
    }

    grafo = malloc(sizeof(GrafoLista));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->n = n;

    grafo->adj = calloc(n, sizeof(No *));

    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }

    return grafo;
}

void inserir_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *novo;

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

    if (sao_adjacentes_lista(grafo, u, v)) {
        return;
    }

    novo = malloc(sizeof(No));

    if (novo == NULL) {
        return;
    }

    novo->destino = v;
    novo->prox = grafo->adj[u];
    grafo->adj[u] = novo;

    novo = malloc(sizeof(No));

    if (novo == NULL) {
        remover_aresta_lista(grafo, u, v);
        return;
    }

    novo->destino = u;
    novo->prox = grafo->adj[v];
    grafo->adj[v] = novo;
}

void remover_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *atual;
    No *anterior;

    if (grafo == NULL) {
        return;
    }

    if (u < 0 || u >= grafo->n ||
        v < 0 || v >= grafo->n) {
        return;
    }

    atual = grafo->adj[u];
    anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == v) {
            if (anterior == NULL) {
                grafo->adj[u] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            break;
        }

        anterior = atual;
        atual = atual->prox;
    }

    atual = grafo->adj[v];
    anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == u) {
            if (anterior == NULL) {
                grafo->adj[v] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            break;
        }

        anterior = atual;
        atual = atual->prox;
    }
}

int grau_lista(GrafoLista *grafo, int vertice) {
    No *atual;
    int grau = 0;

    if (grafo == NULL) {
        return -1;
    }

    if (vertice < 0 || vertice >= grafo->n) {
        return -1;
    }

    atual = grafo->adj[vertice];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int sao_adjacentes_lista(GrafoLista *grafo, int u, int v) {
    No *atual;

    if (grafo == NULL) {
        return 0;
    }

    if (u < 0 || u >= grafo->n ||
        v < 0 || v >= grafo->n) {
        return 0;
    }

    atual = grafo->adj[u];

    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

void liberar_grafo_lista(GrafoLista *grafo) {
    if (grafo == NULL) {
        return;
    }

    for (int i = 0; i < grafo->n; i++) {
        No *atual = grafo->adj[i];

        while (atual != NULL) {
            No *temp = atual;

            atual = atual->prox;

            free(temp);
        }
    }

    free(grafo->adj);
    free(grafo);
}