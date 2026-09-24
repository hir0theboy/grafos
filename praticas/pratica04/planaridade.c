#include <stdlib.h>
#include "planaridade.h"

static int possui_aresta(GrafoLista *g, int u, int v) {
    for (Aresta *a = g->adj[u]; a; a = a->prox)
        if (a->destino == v)
            return 1;
    return 0;
}

static int contar_arestas(GrafoLista *g) {
    int soma = 0;

    for (int i = 0; i < g->n; i++)
        for (Aresta *a = g->adj[i]; a; a = a->prox)
            soma++;

    return soma / 2;
}

int eh_planar_euler(GrafoLista *g) {
    int n = g->n;
    int m = contar_arestas(g);

    if (n < 3)
        return 1;

    return m <= 3 * n - 6;
}

static int conexao_subconjunto(GrafoLista *g, int *grupo, int tamanho) {
    if (tamanho <= 1)
        return 1;

    int *visitado = calloc(g->n, sizeof(int));
    int *fila = malloc(g->n * sizeof(int));
    int inicio = -1;
    int total = 0;

    for (int i = 0; i < g->n; i++) {
        if (grupo[i]) {
            inicio = i;
            break;
        }
    }

    int frente = 0, tras = 0;
    fila[tras++] = inicio;
    visitado[inicio] = 1;

    while (frente < tras) {
        int u = fila[frente++];
        total++;

        for (int v = 0; v < g->n; v++) {
            if (grupo[v] && !visitado[v] && possui_aresta(g, u, v)) {
                visitado[v] = 1;
                fila[tras++] = v;
            }
        }
    }

    free(visitado);
    free(fila);

    return total == tamanho;
}

static int grupos_adjacentes(GrafoLista *g, int *a, int *b) {
    for (int u = 0; u < g->n; u++) {
        if (!a[u]) continue;

        for (Aresta *aresta = g->adj[u]; aresta; aresta = aresta->prox) {
            if (b[aresta->destino])
                return 1;
        }
    }

    return 0;
}

static int gerar_particoes(GrafoLista *g, int tipo, int indice, int *grupo,
                           int *grupos, int total_grupos) {
    if (indice == g->n) {
        int tamanhos[6] = {0};

        for (int i = 0; i < g->n; i++) {
            if (grupo[i] < 0 || grupo[i] >= total_grupos)
                return 0;
            tamanhos[grupo[i]]++;
        }

        for (int i = 0; i < total_grupos; i++) {
            int *sub = calloc(g->n, sizeof(int));

            for (int j = 0; j < g->n; j++)
                if (grupo[j] == i)
                    sub[j] = 1;

            int conectado = conexao_subconjunto(g, sub, tamanhos[i]);
            free(sub);

            if (!conectado)
                return 0;
        }

        for (int i = 0; i < total_grupos; i++) {
            for (int j = i + 1; j < total_grupos; j++) {
                if (!grupos[i * total_grupos + j])
                    continue;

                int *a = calloc(g->n, sizeof(int));
                int *b = calloc(g->n, sizeof(int));

                for (int k = 0; k < g->n; k++) {
                    if (grupo[k] == i) a[k] = 1;
                    if (grupo[k] == j) b[k] = 1;
                }

                if (!grupos_adjacentes(g, a, b)) {
                    free(a);
                    free(b);
                    return 0;
                }

                free(a);
                free(b);
            }
        }

        return 1;
    }

    for (int i = 0; i < total_grupos; i++) {
        grupo[indice] = i;

        int usados = 0;
        for (int j = 0; j <= indice; j++) {
            if (grupo[j] == i)
                usados++;
        }

        if (usados == 1 && i > indice)
            continue;

        if (gerar_particoes(g, tipo, indice + 1, grupo, grupos, total_grupos))
            return 1;
    }

    return 0;
}

static int possui_k5(GrafoLista *g) {
    if (g->n < 5)
        return 0;

    int grupos[25] = {0};

    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            grupos[i * 5 + j] = 1;

    int *grupo = malloc(g->n * sizeof(int));

    for (int i = 0; i < g->n; i++)
        grupo[i] = -1;

    int resultado = gerar_particoes(g, 5, 0, grupo, grupos, 5);

    free(grupo);
    return resultado;
}

static int possui_k33(GrafoLista *g) {
    if (g->n < 6)
        return 0;

    int grupos[36] = {0};

    for (int i = 0; i < 3; i++)
        for (int j = 3; j < 6; j++)
            grupos[i * 6 + j] = 1;

    int *grupo = malloc(g->n * sizeof(int));

    for (int i = 0; i < g->n; i++)
        grupo[i] = -1;

    int resultado = gerar_particoes(g, 6, 0, grupo, grupos, 6);

    free(grupo);
    return resultado;
}

int eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g))
        return 0;

    if (g->n <= 10) {
        if (possui_k5(g) || possui_k33(g))
            return 0;
    }

    return 1;
}
