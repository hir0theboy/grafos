#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

static void imprimir_vetor(const char *rotulo, int *vetor, int n) {
    printf("%s:", rotulo);
    for (int i = 0; i < n; i++) {
        printf(" %d", vetor[i]);
    }
    printf("\n");
}

int main(void) {

    int n = 8;
    GrafoLista *g = grafo_criar(n, 0);

    grafo_add_aresta(g, 0, 1);
    grafo_add_aresta(g, 0, 2);
    grafo_add_aresta(g, 1, 3);
    grafo_add_aresta(g, 2, 3);
    grafo_add_aresta(g, 4, 5);
    grafo_add_aresta(g, 6, 7);

    printf("=== Lista de adjacencia ===\n");
    grafo_imprimir(g);
    printf("\n");


    int *dist = malloc(sizeof(int) * n);
    int *pred_bfs = malloc(sizeof(int) * n);
    bfs(g, 0, dist, pred_bfs);

    printf("=== BFS a partir do vertice 0 ===\n");
    imprimir_vetor("Distancias   ", dist, n);
    imprimir_vetor("Predecessores", pred_bfs, n);
    printf("\n");

    int *visitado = calloc(n, sizeof(int));
    int *entrada = malloc(sizeof(int) * n);
    int *saida = malloc(sizeof(int) * n);
    int *pred_dfs = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) pred_dfs[i] = -1;
    int tempo = 0;

    printf("=== DFS recursiva (floresta completa) ===\n");
    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            dfs_recursiva(g, i, visitado, &tempo, entrada, saida, pred_dfs);
        }
    }
    imprimir_vetor("Entrada", entrada, n);
    imprimir_vetor("Saida  ", saida, n);
    printf("\n");

    printf("Numero de componentes conexos: %d\n\n", contar_componentes(g));

    printf("O grafo eh bipartido? %s\n\n", eh_bipartido(g) ? "Sim" : "Nao");


    printf("O grafo (nao dirigido) tem ciclo? %s\n\n", tem_ciclo(g) ? "Sim" : "Nao");

    grafo_destruir(g);
    free(dist);
    free(pred_bfs);
    free(visitado);
    free(entrada);
    free(saida);
    free(pred_dfs);


    GrafoLista *g_bip = grafo_criar(5, 0);
    grafo_add_aresta(g_bip, 0, 1);
    grafo_add_aresta(g_bip, 0, 2);
    grafo_add_aresta(g_bip, 1, 3);
    grafo_add_aresta(g_bip, 1, 4);

    printf("=== Arvore (sem ciclos) ===\n");
    grafo_imprimir(g_bip);
    printf("Eh bipartido? %s\n", eh_bipartido(g_bip) ? "Sim" : "Nao");
    printf("Tem ciclo?    %s\n\n", tem_ciclo(g_bip) ? "Sim" : "Nao");
    grafo_destruir(g_bip);


    GrafoLista *g_dir = grafo_criar(3, 1);
    grafo_add_aresta(g_dir, 0, 1);
    grafo_add_aresta(g_dir, 1, 2);
    grafo_add_aresta(g_dir, 2, 0); 

    printf("=== Grafo dirigido com ciclo ===\n");
    grafo_imprimir(g_dir);
    printf("Tem ciclo? %s\n", tem_ciclo(g_dir) ? "Sim" : "Nao");
    grafo_destruir(g_dir);

    return 0;
}