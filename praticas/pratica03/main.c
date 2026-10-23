#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

static void imprimir_ordem(const char *rotulo, int *ordem, int tamanho) {
    if (ordem == NULL) {
        printf("%s: (grafo possui ciclo - ordenacao impossivel)\n", rotulo);
        return;
    }
    printf("%s:", rotulo);
    for (int i = 0; i < tamanho; i++) {
        printf(" %d", ordem[i]);
    }
    printf("\n");
}

int main(void) {

    int n = 7;
    GrafoLista *g = grafo_criar(n, 1); 

    grafo_add_aresta(g, 0, 1); /* cueca -> calca */
    grafo_add_aresta(g, 1, 2); /* calca -> cinto */
    grafo_add_aresta(g, 3, 4); /* camisa -> jaqueta */
    grafo_add_aresta(g, 2, 4); /* cinto -> jaqueta */
    grafo_add_aresta(g, 5, 6); /* meia -> sapato */
    grafo_add_aresta(g, 1, 6); /* calca -> sapato */

    printf("=== Lista de adjacencia (DAG) ===\n");
    grafo_imprimir(g);
    printf("\n");

    printf("O grafo eh um DAG? %s\n\n", eh_dag(g) ? "Sim" : "Nao");

    int tamanho_kahn = 0;
    int *ordem_kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);
    imprimir_ordem("Ordenacao topologica (Kahn)", ordem_kahn, tamanho_kahn);

    int tamanho_dfs = 0;
    int *ordem_dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);
    imprimir_ordem("Ordenacao topologica (DFS) ", ordem_dfs, tamanho_dfs);

    free(ordem_kahn);
    free(ordem_dfs);
    grafo_destruir(g);
    printf("\n");

    GrafoLista *g_ciclo = grafo_criar(3, 1);
    grafo_add_aresta(g_ciclo, 0, 1);
    grafo_add_aresta(g_ciclo, 1, 2);
    grafo_add_aresta(g_ciclo, 2, 0); 

    printf("=== Lista de adjacencia (com ciclo) ===\n");
    grafo_imprimir(g_ciclo);
    printf("\n");

    printf("O grafo eh um DAG? %s\n\n", eh_dag(g_ciclo) ? "Sim" : "Nao");

    int tamanho_kahn2 = 0;
    int *ordem_kahn2 = ordenacao_topologica_kahn(g_ciclo, &tamanho_kahn2);
    imprimir_ordem("Ordenacao topologica (Kahn)", ordem_kahn2, tamanho_kahn2);

    int tamanho_dfs2 = 0;
    int *ordem_dfs2 = ordenacao_topologica_dfs(g_ciclo, &tamanho_dfs2);
    imprimir_ordem("Ordenacao topologica (DFS) ", ordem_dfs2, tamanho_dfs2);

    free(ordem_kahn2);
    free(ordem_dfs2);
    grafo_destruir(g_ciclo);

    return 0;
}