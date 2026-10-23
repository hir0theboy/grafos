#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "grafo_lista.h"

typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

Fila *fila_criar(int capacidade);
void fila_destruir(Fila *f);
int fila_vazia(const Fila *f);
int fila_cheia(const Fila *f);
void fila_inserir(Fila *f, int valor);
int fila_remover(Fila *f);

void bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif