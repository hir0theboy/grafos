#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

static GrafoAGM *montar_grafo(int n, const Aresta *arestas, int m) {
    GrafoAGM *g = criar_grafo_agm(n);
    for (int i = 0; i < m; i++)
        adicionar_aresta_agm(g, arestas[i].u, arestas[i].v, arestas[i].peso);
    return g;
}

static void testar(const char *titulo, int n, const Aresta *arestas, int m) {
    printf("=== %s (V = %d, E = %d) ===\n", titulo, n, m);

    /* Kruskal */
    Aresta *mst = malloc((size_t)(n > 1 ? n - 1 : 1) * sizeof(Aresta));
    int qtd = 0;
    long peso_k = kruskal(n, arestas, m, mst, &qtd);

    printf("Kruskal:\n");
    for (int i = 0; i < qtd; i++)
        printf("  %d - %d (peso %d)\n", mst[i].u, mst[i].v, mst[i].peso);
    printf("  Peso total: %ld\n", peso_k);
    if (qtd < n - 1)
        printf("  Grafo desconexo: floresta geradora com %d arestas.\n", qtd);

    /* Prim a partir do vértice 0 */
    GrafoAGM *g = montar_grafo(n, arestas, m);
    int *pai = malloc((size_t)n * sizeof(int));
    int *chave = malloc((size_t)n * sizeof(int));
    long peso_p = prim(g, 0, pai, chave);

    printf("Prim (origem 0):\n");
    int alcancados = 1;
    for (int v = 0; v < n; v++) {
        if (pai[v] != -1) {
            printf("  %d - %d (peso %d)\n", pai[v], v, chave[v]);
            alcancados++;
        }
    }
    printf("  Peso total: %ld\n", peso_p);
    if (alcancados < n)
        printf("  Apenas %d de %d vertices alcancados a partir da origem.\n", alcancados, n);

    if (qtd == n - 1)
        printf("Kruskal e Prim %s.\n\n", peso_k == peso_p ? "concordam no peso total" : "DIVERGEM (erro!)");
    else
        printf("\n");

    free(mst);
    free(pai);
    free(chave);
    liberar_grafo_agm(g);
}

int main(void) {
    /* Teste 1: grafo clássico com 9 vértices (a..i = 0..8), AGM de peso 37 */
    Aresta g1[] = {
        {0, 1, 4},  {0, 7, 8},  {1, 7, 11}, {1, 2, 8},  {2, 3, 7},
        {2, 5, 4},  {2, 8, 2},  {3, 4, 9},  {3, 5, 14}, {4, 5, 10},
        {5, 6, 2},  {6, 7, 1},  {6, 8, 6},  {7, 8, 7}
    };
    testar("Teste 1: grafo conexo", 9, g1, (int)(sizeof(g1) / sizeof(g1[0])));

    /* Teste 2: grafo pequeno com arestas de mesmo peso */
    Aresta g2[] = {
        {0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 0, 1}, {0, 2, 5}
    };
    testar("Teste 2: pesos repetidos", 4, g2, (int)(sizeof(g2) / sizeof(g2[0])));

    /* Teste 3: grafo desconexo (componentes {0,1,2}, {3,4} e {5} isolado) */
    Aresta g3[] = {
        {0, 1, 1}, {1, 2, 2}, {0, 2, 3}, {3, 4, 4}
    };
    testar("Teste 3: grafo desconexo", 6, g3, (int)(sizeof(g3) / sizeof(g3[0])));

    return 0;
}