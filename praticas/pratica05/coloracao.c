#include <stdlib.h>
#include "coloracao.h"

static int grau(GrafoLista *g, int u) {
    int cont = 0;

    for (Aresta *a = g->adj[u]; a; a = a->prox)
        cont++;

    return cont;
}

static int escolher_cor(GrafoLista *g, int u, int *cor) {
    int *usada = calloc(g->n + 1, sizeof(int));

    for (Aresta *a = g->adj[u]; a; a = a->prox) {
        int v = a->destino;

        if (cor[v] >= 0)
            usada[cor[v]] = 1;
    }

    int c = 0;

    while (usada[c])
        c++;

    free(usada);
    return c;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    if (!g || !num_cores)
        return NULL;

    int *cor = malloc(g->n * sizeof(int));

    if (!cor)
        return NULL;

    for (int i = 0; i < g->n; i++)
        cor[i] = -1;

    int maior_cor = -1;

    for (int u = 0; u < g->n; u++) {
        cor[u] = escolher_cor(g, u, cor);

        if (cor[u] > maior_cor)
            maior_cor = cor[u];
    }

    *num_cores = maior_cor + 1;

    return cor;
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    if (!g || !num_cores)
        return NULL;

    int *ordem = malloc(g->n * sizeof(int));
    int *cor = malloc(g->n * sizeof(int));

    if (!ordem || !cor) {
        free(ordem);
        free(cor);
        return NULL;
    }

    for (int i = 0; i < g->n; i++) {
        ordem[i] = i;
        cor[i] = -1;
    }

    for (int i = 0; i < g->n - 1; i++) {
        for (int j = i + 1; j < g->n; j++) {
            int grau_i = grau(g, ordem[i]);
            int grau_j = grau(g, ordem[j]);

            if (grau_j > grau_i) {
                int temp = ordem[i];
                ordem[i] = ordem[j];
                ordem[j] = temp;
            }
        }
    }

    int maior_cor = -1;

    for (int i = 0; i < g->n; i++) {
        int u = ordem[i];

        cor[u] = escolher_cor(g, u, cor);

        if (cor[u] > maior_cor)
            maior_cor = cor[u];
    }

    *num_cores = maior_cor + 1;

    free(ordem);

    return cor;
}

int eh_bipartido(GrafoLista *g) {
    if (!g)
        return 0;

    int *cor = malloc(g->n * sizeof(int));

    if (!cor)
        return 0;

    for (int i = 0; i < g->n; i++)
        cor[i] = -1;

    int *fila = malloc(g->n * sizeof(int));

    if (!fila) {
        free(cor);
        return 0;
    }

    for (int inicio = 0; inicio < g->n; inicio++) {
        if (cor[inicio] != -1)
            continue;

        int frente = 0;
        int tras = 0;

        fila[tras++] = inicio;
        cor[inicio] = 0;

        while (frente < tras) {
            int u = fila[frente++];

            for (Aresta *a = g->adj[u]; a; a = a->prox) {
                int v = a->destino;

                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[tras++] = v;
                } else if (cor[v] == cor[u]) {
                    free(fila);
                    free(cor);
                    return 0;
                }
            }
        }
    }

    free(fila);
    free(cor);

    return 1;
}
