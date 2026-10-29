#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

/* ===================== Grafo ===================== */

GrafoCobertura *criar_grafo_cob(int n) {
    GrafoCobertura *g = malloc(sizeof(GrafoCobertura));
    if (!g) return NULL;
    g->n = n;
    g->m = 0;
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

void adicionar_aresta_cob(GrafoCobertura *g, int u, int v) {
    if (u == v || g->adj[u][v]) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
    g->m++;
}

void liberar_grafo_cob(GrafoCobertura *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

int grau_cob(const GrafoCobertura *g, int v) {
    int d = 0;
    for (int j = 0; j < g->n; j++) d += g->adj[v][j];
    return d;
}

/* ===================== Verificação ===================== */

int eh_cobertura(const GrafoCobertura *g, const int *solucao) {
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            if (g->adj[u][v] && !solucao[u] && !solucao[v])
                return 0;   /* aresta u-v descoberta */
    return 1;
}

/* ===================== Backtracking com poda ===================== */

typedef struct {
    const GrafoCobertura *g;
    int *incluido;     /* decisões parciais: 1 = vértice na cobertura */
    int *melhor;       /* melhor cobertura encontrada */
    int melhor_tam;
    int max_grau;
    long long nos;
} CtxBT;

/*
 * Decide os vértices em ordem (0, 1, 2, ...): cada um entra ou não na cobertura.
 *   i            = próximo vértice a decidir
 *   tam          = vértices já incluídos
 *   nao_cobertas = arestas ainda descobertas
 */
static void bt(CtxBT *c, int i, int tam, int nao_cobertas) {
    const GrafoCobertura *g = c->g;
    c->nos++;

    if (nao_cobertas == 0) {   /* tudo coberto: guarda se for melhor */
        if (tam < c->melhor_tam) {
            c->melhor_tam = tam;
            memcpy(c->melhor, c->incluido, (size_t)g->n * sizeof(int));
        }
        return;
    }
    if (i == g->n) return;

    /* Poda por limite: cada vértice cobre no máximo max_grau arestas, então
     * faltam pelo menos ceil(nao_cobertas / max_grau) vértices. */
    int minimo = (nao_cobertas + c->max_grau - 1) / c->max_grau;
    if (tam + minimo >= c->melhor_tam) return;

    /* Ramo 1: incluir i. Cobre as arestas i-j cujo outro extremo ainda não está dentro. */
    int cobre = 0;
    for (int j = 0; j < g->n; j++)
        if (g->adj[i][j] && !c->incluido[j]) cobre++;
    c->incluido[i] = 1;
    bt(c, i + 1, tam + 1, nao_cobertas - cobre);
    c->incluido[i] = 0;

    /* Ramo 2: excluir i. Só é viável se todos os vizinhos anteriores estão
     * na cobertura (senão a aresta i-j ficaria descoberta para sempre). */
    for (int j = 0; j < i; j++)
        if (g->adj[i][j] && !c->incluido[j]) return;
    bt(c, i + 1, tam, nao_cobertas);
}

int cobertura_minima_bt(const GrafoCobertura *g, int *solucao, long long *nos) {
    int n = g->n;
    CtxBT c;
    c.g = g;
    c.incluido = calloc((size_t)n, sizeof(int));
    c.melhor = solucao;
    c.melhor_tam = n;          /* incluir todos os vértices sempre é uma cobertura */
    c.nos = 0;
    c.max_grau = 1;
    for (int v = 0; v < n; v++) {
        solucao[v] = 1;
        int d = grau_cob(g, v);
        if (d > c.max_grau) c.max_grau = d;
    }

    bt(&c, 0, 0, g->m);

    if (nos) *nos = c.nos;
    int tam = c.melhor_tam;
    free(c.incluido);
    return tam;
}

/* ===================== Heurística gulosa ===================== */

int cobertura_gulosa(const GrafoCobertura *g, int *solucao) {
    int n = g->n;
    int *grau = malloc((size_t)n * sizeof(int));   /* grau restante (arestas descobertas) */
    int restantes = g->m;
    int tam = 0;

    for (int v = 0; v < n; v++) {
        solucao[v] = 0;
        grau[v] = grau_cob(g, v);
    }

    while (restantes > 0) {
        int melhor = -1;
        for (int v = 0; v < n; v++)
            if (!solucao[v] && (melhor == -1 || grau[v] > grau[melhor]))
                melhor = v;

        solucao[melhor] = 1;
        tam++;
        restantes -= grau[melhor];
        for (int j = 0; j < n; j++)
            if (g->adj[melhor][j] && !solucao[j]) grau[j]--;
        grau[melhor] = 0;
    }

    free(grau);
    return tam;
}