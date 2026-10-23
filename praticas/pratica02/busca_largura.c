#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"


Fila *fila_criar(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(sizeof(int) * capacidade);
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

void fila_destruir(Fila *f) {
    if (!f) return;
    free(f->dados);
    free(f);
}

int fila_vazia(const Fila *f) {
    return f->tamanho == 0;
}

int fila_cheia(const Fila *f) {
    return f->tamanho == f->capacidade;
}

void fila_inserir(Fila *f, int valor) {
    if (fila_cheia(f)) {
        fprintf(stderr, "Erro: fila cheia\n");
        return;
    }
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int fila_remover(Fila *f) {
    if (fila_vazia(f)) {
        fprintf(stderr, "Erro: fila vazia\n");
        return -1;
    }
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila *f = fila_criar(n);
    visitado[origem] = 1;
    dist[origem] = 0;
    fila_inserir(f, origem);

    while (!fila_vazia(f)) {
        int u = fila_remover(f);
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            int v = atual->destino;
            if (!visitado[v]) {
                visitado[v] = 1;
                dist[v] = dist[u] + 1;
                pred[v] = u;
                fila_inserir(f, v);
            }
        }
    }

    fila_destruir(f);
    free(visitado);
}