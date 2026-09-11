#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista *grafo_criar(int num_vertices, int dirigido) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->dirigido = dirigido;
    g->adj = calloc(num_vertices, sizeof(No *));
    return g;
}

static void inserir_no_inicio(No **lista, int destino) {
    No *novo = malloc(sizeof(No));
    novo->destino = destino;
    novo->prox = *lista;
    *lista = novo;
}

void grafo_add_aresta(GrafoLista *g, int origem, int destino) {
    inserir_no_inicio(&g->adj[origem], destino);
    if (!g->dirigido) {
        inserir_no_inicio(&g->adj[destino], origem);
    }
}

void grafo_destruir(GrafoLista *g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->adj[i];
        while (atual) {
            No *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

void grafo_imprimir(const GrafoLista *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d:", i);
        for (No *atual = g->adj[i]; atual != NULL; atual = atual->prox) {
            printf(" -> %d", atual->destino);
        }
        printf("\n");
    }
}