#include <stdlib.h>
#include "hamiltoniano.h"

/* ===================== Grafo ===================== */

GrafoHam *criar_grafo_ham(int n) {
    GrafoHam *g = malloc(sizeof(GrafoHam));
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)n, sizeof(char *));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        g->adj[i] = calloc((size_t)n, sizeof(char));
        if (!g->adj[i]) {
            for (int j = 0; j < i; j++) free(g->adj[j]);
            free(g->adj);
            free(g);
            return NULL;
        }
    }
    return g;
}

void adicionar_aresta_ham(GrafoHam *g, int u, int v) {
    if (u == v) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void liberar_grafo_ham(GrafoHam *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

/* ===================== Caminho Hamiltoniano ===================== */

/* Estende o caminho parcial caminho[0..pos-1] com vértices ainda não usados. */
static int dfs_caminho(const GrafoHam *g, int *caminho, char *usado, int pos) {
    if (pos == g->n) return 1;   /* todos os vértices visitados */

    int ultimo = caminho[pos - 1];
    for (int v = 0; v < g->n; v++) {
        if (!usado[v] && g->adj[ultimo][v]) {
            usado[v] = 1;
            caminho[pos] = v;
            if (dfs_caminho(g, caminho, usado, pos + 1)) return 1;
            usado[v] = 0;        /* backtrack */
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const GrafoHam *g, int *caminho) {
    int n = g->n;
    if (n <= 0) return 0;

    char *usado = calloc((size_t)n, sizeof(char));
    if (!usado) return 0;

    int achou = 0;
    for (int inicio = 0; inicio < n && !achou; inicio++) {
        for (int i = 0; i < n; i++) usado[i] = 0;
        usado[inicio] = 1;
        caminho[0] = inicio;
        achou = dfs_caminho(g, caminho, usado, 1);
    }

    free(usado);
    return achou;
}

/* ===================== Ciclo Hamiltoniano ===================== */

static int dfs_ciclo(const GrafoHam *g, int *ciclo, char *usado, int pos) {
    if (pos == g->n)             /* o último precisa fechar o ciclo com o vértice 0 */
        return g->adj[ciclo[pos - 1]][ciclo[0]];

    int ultimo = ciclo[pos - 1];
    for (int v = 1; v < g->n; v++) {
        if (!usado[v] && g->adj[ultimo][v]) {
            usado[v] = 1;
            ciclo[pos] = v;
            if (dfs_ciclo(g, ciclo, usado, pos + 1)) return 1;
            usado[v] = 0;
        }
    }
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(const GrafoHam *g, int *ciclo) {
    int n = g->n;
    if (n < 3) return 0;

    char *usado = calloc((size_t)n, sizeof(char));
    if (!usado) return 0;

    usado[0] = 1;   /* o ciclo é o mesmo em qualquer rotação: fixa a origem em 0 */
    ciclo[0] = 0;
    int achou = dfs_ciclo(g, ciclo, usado, 1);

    free(usado);
    return achou;
}
