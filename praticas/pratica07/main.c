#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void imprimir_caminho(int *pred, int v) {
    if (pred[v] != -1) {
        imprimir_caminho(pred, pred[v]);
        printf(" -> ");
    }
    printf("%d", v);
}

static void executar_teste(const char *titulo, GrafoListaPonderado *g, int origem) {
    int n = g->n;
    int *dist = malloc((size_t)n * sizeof(int));
    int *pred = malloc((size_t)n * sizeof(int));

    printf("=== %s (origem = %d) ===\n", titulo, origem);

    if (bellman_ford(g, origem, dist, pred)) {
        printf("Sem ciclo negativo. Menores distancias:\n");
        for (int v = 0; v < n; v++) {
            printf("  %d: ", v);
            if (dist[v] == INF) {
                printf("inalcancavel\n");
            } else {
                printf("dist = %d, caminho = ", dist[v]);
                imprimir_caminho(pred, v);
                printf("\n");
            }
        }
    } else {
        printf("Ciclo de custo negativo detectado!\n");
        reconstruir_ciclo_negativo(g, dist, pred);
    }
    printf("\n");

    free(dist);
    free(pred);
}

int main(void) {
    /* Teste 1: grafo sem ciclo negativo (exemplo clássico, 5 vértices) */
    GrafoListaPonderado *g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1, 6);
    adicionar_aresta(g1, 0, 3, 7);
    adicionar_aresta(g1, 1, 2, 5);
    adicionar_aresta(g1, 1, 3, 8);
    adicionar_aresta(g1, 1, 4, -4);
    adicionar_aresta(g1, 2, 1, -2);
    adicionar_aresta(g1, 3, 2, -3);
    adicionar_aresta(g1, 3, 4, 9);
    adicionar_aresta(g1, 4, 0, 2);
    adicionar_aresta(g1, 4, 2, 7);
    executar_teste("Teste 1: sem ciclo negativo", g1, 0);
    liberar_grafo(g1);

    /* Teste 2: ciclo negativo 1 -> 2 -> 3 -> 1 (peso total -1) */
    GrafoListaPonderado *g2 = criar_grafo(5);
    adicionar_aresta(g2, 0, 1, 4);
    adicionar_aresta(g2, 1, 2, 1);
    adicionar_aresta(g2, 2, 3, -3);
    adicionar_aresta(g2, 3, 1, 1);
    adicionar_aresta(g2, 3, 4, 2);
    executar_teste("Teste 2: com ciclo negativo", g2, 0);
    liberar_grafo(g2);

    /* Teste 3: vértices inalcançáveis da origem */
    GrafoListaPonderado *g3 = criar_grafo(4);
    adicionar_aresta(g3, 0, 1, 3);
    adicionar_aresta(g3, 1, 2, -1);
    adicionar_aresta(g3, 3, 0, 1);
    executar_teste("Teste 3: vertice inalcancavel", g3, 0);
    liberar_grafo(g3);

    return 0;
}