#include <stdio.h>
#include <stdlib.h>
#include "dag.h"


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

static int dfs_tem_ciclo(GrafoLista *g, int u, int *estado) {
    estado[u] = 1;
    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (estado[v] == 1) {
            return 1; 
        }
        if (estado[v] == 0 && dfs_tem_ciclo(g, v, estado)) {
            return 1;
        }
    }
    estado[u] = 2;
    return 0;
}

int eh_dag(GrafoLista *g) {
    int n = g->num_vertices;
    int *estado = calloc(n, sizeof(int));
    int ciclo = 0;

    for (int i = 0; i < n && !ciclo; i++) {
        if (estado[i] == 0) {
            ciclo = dfs_tem_ciclo(g, i, estado);
        }
    }

    free(estado);
    return !ciclo;
}

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int *grau_entrada = calloc(n, sizeof(int));

    for (int u = 0; u < n; u++) {
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            grau_entrada[atual->destino]++;
        }
    }

    int *fila = malloc(sizeof(int) * n);
    int inicio = 0, fim = 0;

    for (int i = 0; i < n; i++) {
        if (grau_entrada[i] == 0) {
            fila[fim++] = i;
        }
    }

    int *ordem = malloc(sizeof(int) * n);
    int qtd = 0;

    while (inicio < fim) {
        int u = fila[inicio++];
        ordem[qtd++] = u;

        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            int v = atual->destino;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
        }
    }

    free(grau_entrada);
    free(fila);

    if (qtd != n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = qtd;
    return ordem;
}

static int dfs_topologica_aux(GrafoLista *g, int u, int *estado, int *pilha, int *topo) {
    estado[u] = 1;
    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (estado[v] == 1) {
            return 1;
        }
        if (estado[v] == 0 && dfs_topologica_aux(g, v, estado, pilha, topo)) {
            return 1;
        }
    }
    estado[u] = 2;
    pilha[(*topo)++] = u;
    return 0;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int *estado = calloc(n, sizeof(int));
    int *pilha = malloc(sizeof(int) * n);
    int topo = 0;
    int ciclo = 0;

    for (int i = 0; i < n && !ciclo; i++) {
        if (estado[i] == 0) {
            ciclo = dfs_topologica_aux(g, i, estado, pilha, &topo);
        }
    }

    free(estado);

    if (ciclo) {
        free(pilha);
        *tamanho = 0;
        return NULL;
    }

    int *ordem = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        ordem[i] = pilha[n - 1 - i];
    }

    free(pilha);
    *tamanho = n;
    return ordem;
}