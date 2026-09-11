#include <stdio.h>
#include <stdlib.h>
#include "busca_profundidade.h"


Pilha *pilha_criar(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    p->dados = malloc(sizeof(int) * capacidade);
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

void pilha_destruir(Pilha *p) {
    if (!p) return;
    free(p->dados);
    free(p);
}

int pilha_vazia(const Pilha *p) {
    return p->topo == -1;
}

int pilha_cheia(const Pilha *p) {
    return p->topo == p->capacidade - 1;
}

void pilha_empilhar(Pilha *p, int valor) {
    if (pilha_cheia(p)) {
        fprintf(stderr, "Erro: pilha cheia\n");
        return;
    }
    p->dados[++p->topo] = valor;
}

int pilha_desempilhar(Pilha *p) {
    if (pilha_vazia(p)) {
        fprintf(stderr, "Erro: pilha vazia\n");
        return -1;
    }
    return p->dados[p->topo--];
}

/* ---------------- DFS recursiva com tempos ---------------- */

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo,
                    int *entrada, int *saida, int *pred) {
    visitado[u] = 1;
    entrada[u] = (*tempo)++;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (!visitado[v]) {
            pred[v] = u;
            dfs_recursiva(g, v, visitado, tempo, entrada, saida, pred);
        }
    }

    saida[u] = (*tempo)++;
}


void dfs_iterativa(GrafoLista *g, int origem, int *visitado, int *pred) {

    int capacidade = g->num_vertices * g->num_vertices + 1;
    Pilha *p = pilha_criar(capacidade);
    pilha_empilhar(p, origem);

    while (!pilha_vazia(p)) {
        int u = pilha_desempilhar(p);
        if (!visitado[u]) {
            visitado[u] = 1;
            for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
                int v = atual->destino;
                if (!visitado[v]) {
                    pred[v] = u;
                    pilha_empilhar(p, v);
                }
            }
        }
    }

    pilha_destruir(p);
}


int contar_componentes(GrafoLista *g) {
    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));
    int *pred = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) pred[i] = -1;

    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            componentes++;
            dfs_iterativa(g, i, visitado, pred);
        }
    }

    free(visitado);
    free(pred);
    return componentes;
}


int eh_bipartido(GrafoLista *g) {
    int n = g->num_vertices;
    int *cor = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) cor[i] = -1;

    int *fila = malloc(sizeof(int) * n);

    for (int s = 0; s < n; s++) {
        if (cor[s] != -1) continue;

        cor[s] = 0;
        int inicio = 0, fim = 0;
        fila[fim++] = s;

        while (inicio < fim) {
            int u = fila[inicio++];
            for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
                int v = atual->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[fim++] = v;
                } else if (cor[v] == cor[u]) {
                    free(cor);
                    free(fila);
                    return 0;
                }
            }
        }
    }

    free(cor);
    free(fila);
    return 1;
}

static int tem_ciclo_direcionado_aux(GrafoLista *g, int u, int *estado) {
    estado[u] = 1;
    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (estado[v] == 1) {
            return 1; 
        }
        if (estado[v] == 0 && tem_ciclo_direcionado_aux(g, v, estado)) {
            return 1;
        }
    }
    estado[u] = 2;
    return 0;
}

static int tem_ciclo_nao_direcionado_aux(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;
    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (!visitado[v]) {
            if (tem_ciclo_nao_direcionado_aux(g, v, u, visitado)) return 1;
        } else if (v != pai) {
            return 1;
        }
    }
    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int n = g->num_vertices;
    int *estado = calloc(n, sizeof(int));
    int achou = 0;

    if (g->dirigido) {
        for (int i = 0; i < n && !achou; i++) {
            if (estado[i] == 0) {
                achou = tem_ciclo_direcionado_aux(g, i, estado);
            }
        }
    } else {
        for (int i = 0; i < n && !achou; i++) {
            if (!estado[i]) {
                achou = tem_ciclo_nao_direcionado_aux(g, i, -1, estado);
            }
        }
    }

    free(estado);
    return achou;
}