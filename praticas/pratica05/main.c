#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

static void imprimir_cores(const char *nome, int *cor, int n, int num_cores) {
    printf("%s\n", nome);

    for (int i = 0; i < n; i++)
        printf("Vertice %d: cor %d\n", i, cor[i]);

    printf("Numero de cores: %d\n\n", num_cores);
}

static void teste_ciclo(void) {
    GrafoLista *g = criar_grafo(5);

    for (int i = 0; i < 5; i++)
        adicionar_aresta(g, i, (i + 1) % 5);

    int num_cores_gulosa;
    int num_cores_wp;

    int *cores_gulosa = coloracao_gulosa(g, &num_cores_gulosa);
    int *cores_wp = coloracao_welsh_powell(g, &num_cores_wp);

    printf("=== CICLO C5 ===\n");
    imprimir_cores("Coloracao gulosa:", cores_gulosa, g->n, num_cores_gulosa);
    imprimir_cores("Coloracao Welsh-Powell:", cores_wp, g->n, num_cores_wp);
    printf("Bipartido: %s\n\n", eh_bipartido(g) ? "sim" : "nao");

    free(cores_gulosa);
    free(cores_wp);
    liberar_grafo(g);
}

static void teste_bipartido(void) {
    GrafoLista *g = criar_grafo(6);

    adicionar_aresta(g, 0, 3);
    adicionar_aresta(g, 0, 4);
    adicionar_aresta(g, 0, 5);
    adicionar_aresta(g, 1, 3);
    adicionar_aresta(g, 1, 4);
    adicionar_aresta(g, 1, 5);
    adicionar_aresta(g, 2, 3);
    adicionar_aresta(g, 2, 4);
    adicionar_aresta(g, 2, 5);

    int num_cores;
    int *cores = coloracao_welsh_powell(g, &num_cores);

    printf("=== K3,3 ===\n");
    imprimir_cores("Coloracao Welsh-Powell:", cores, g->n, num_cores);
    printf("Bipartido: %s\n\n", eh_bipartido(g) ? "sim" : "nao");

    free(cores);
    liberar_grafo(g);
}

static void teste_triangulo(void) {
    GrafoLista *g = criar_grafo(3);

    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 2, 0);

    int num_cores_gulosa;
    int num_cores_wp;

    int *cores_gulosa = coloracao_gulosa(g, &num_cores_gulosa);
    int *cores_wp = coloracao_welsh_powell(g, &num_cores_wp);

    printf("=== TRIANGULO K3 ===\n");
    imprimir_cores("Coloracao gulosa:", cores_gulosa, g->n, num_cores_gulosa);
    imprimir_cores("Coloracao Welsh-Powell:", cores_wp, g->n, num_cores_wp);
    printf("Bipartido: %s\n\n", eh_bipartido(g) ? "sim" : "nao");

    free(cores_gulosa);
    free(cores_wp);
    liberar_grafo(g);
}

int main(void) {
    teste_ciclo();
    teste_bipartido();
    teste_triangulo();

    return 0;
}
