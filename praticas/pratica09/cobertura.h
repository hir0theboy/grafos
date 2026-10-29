#ifndef COBERTURA_H
#define COBERTURA_H

/* Grafo não direcionado simples, representado por matriz de adjacência. */
typedef struct {
    int n;        /* vértices 0 .. n-1 */
    int m;        /* número de arestas */
    char **adj;   /* adj[u][v] = 1 se existe aresta u-v */
} GrafoCobertura;

GrafoCobertura *criar_grafo_cob(int n);
void adicionar_aresta_cob(GrafoCobertura *g, int u, int v); /* ignora laços e duplicatas */
void liberar_grafo_cob(GrafoCobertura *g);
int grau_cob(const GrafoCobertura *g, int v);

/*
 * Verifica se 'solucao' (solucao[v] = 1 se v está na cobertura) cobre todas
 * as arestas. Retorna 1 se sim, 0 caso contrário.
 */
int eh_cobertura(const GrafoCobertura *g, const int *solucao);

/*
 * Cobertura mínima exata por backtracking com poda por limite.
 * solucao[] deve ter espaço para g->n inteiros.
 * nos (opcional, pode ser NULL) recebe o número de nós visitados na recursão.
 * Retorna o tamanho da cobertura mínima.
 */
int cobertura_minima_bt(const GrafoCobertura *g, int *solucao, long long *nos);

/*
 * Heurística gulosa: enquanto houver aresta descoberta, escolhe o vértice de
 * maior grau restante. Não garante o ótimo. Retorna o tamanho da cobertura.
 */
int cobertura_gulosa(const GrafoCobertura *g, int *solucao);

#endif