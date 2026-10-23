#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"
#include "planaridade.h"

static void imprimir_articulacoes(GrafoLista *g) {
    int *articulacao = malloc(g->n * sizeof(int));
    detectar_articulacoes(g, articulacao);

    printf("Vertices de corte: ");

    int encontrou = 0;
    for (int i = 0; i < g->n; i++) {
        if (articulacao[i]) {
            printf("%d ", i);
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("nenhum");

    printf("\n");
    free(articulacao);
}

static void teste_conectividade(void) {
    GrafoLista *g = criar_grafo(7);

    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 1, 3);
    adicionar_aresta(g, 3, 4);
    adicionar_aresta(g, 3, 5);
    adicionar_aresta(g, 5, 6);

    printf("=== CONECTIVIDADE ===\n");
    imprimir_articulacoes(g);

    printf("Pontes: ");
    detectar_pontes(g);

    liberar_grafo(g);
}

static void teste_planaridade(void) {
    GrafoLista *k5 = criar_grafo(5);

    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            adicionar_aresta(k5, i, j);

    GrafoLista *k33 = criar_grafo(6);

    for (int i = 0; i < 3; i++)
        for (int j = 3; j < 6; j++)
            adicionar_aresta(k33, i, j);

    GrafoLista *ciclo = criar_grafo(5);

    for (int i = 0; i < 5; i++)
        adicionar_aresta(ciclo, i, (i + 1) % 5);

    printf("\n=== PLANARIDADE ===\n");
    printf("K5: %s\n", eh_planar(k5) ? "planar" : "nao planar");
    printf("K3,3: %s\n", eh_planar(k33) ? "planar" : "nao planar");
    printf("Ciclo C5: %s\n", eh_planar(ciclo) ? "planar" : "nao planar");

    liberar_grafo(k5);
    liberar_grafo(k33);
    liberar_grafo(ciclo);
}

int main(void) {
    teste_conectividade();
    teste_planaridade();

    return 0;
}
