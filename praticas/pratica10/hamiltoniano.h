#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* Grafo não direcionado simples, representado por matriz de adjacência. */
typedef struct {
    int n;        /* vértices 0 .. n-1 */
    char **adj;   /* adj[u][v] = 1 se existe aresta u-v */
} GrafoHam;

GrafoHam *criar_grafo_ham(int n);
void adicionar_aresta_ham(GrafoHam *g, int u, int v); /* ignora laços */
void liberar_grafo_ham(GrafoHam *g);

/*
 * Caminho Hamiltoniano por backtracking: visita todos os vértices exatamente
 * uma vez, começando em qualquer vértice. caminho[] deve ter espaço para n
 * inteiros. Retorna 1 se existe (e preenche caminho), 0 caso contrário.
 */
int encontrar_caminho_hamiltoniano_bt(const GrafoHam *g, int *caminho);

/*
 * Ciclo Hamiltoniano por backtracking: caminho Hamiltoniano que começa em 0 e
 * cujo último vértice é adjacente a 0. ciclo[] deve ter espaço para n inteiros
 * (o retorno ao vértice 0 é implícito). Retorna 1 se existe, 0 caso contrário.
 * Um ciclo simples exige n >= 3; para n < 3 retorna 0.
 */
int encontrar_ciclo_hamiltoniano_bt(const GrafoHam *g, int *ciclo);

#endif
