#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tps.h"

/* ===================== Instância ===================== */

InstanciaTSP *criar_instancia_tsp(int n) {
    InstanciaTSP *t = malloc(sizeof(InstanciaTSP));
    if (!t) return NULL;
    t->n = n;
    t->d = calloc((size_t)n, sizeof(int *));
    if (!t->d) {
        free(t);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        t->d[i] = calloc((size_t)n, sizeof(int));
        if (!t->d[i]) {
            for (int j = 0; j < i; j++) free(t->d[j]);
            free(t->d);
            free(t);
            return NULL;
        }
    }
    return t;
}

void liberar_instancia_tsp(InstanciaTSP *t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) free(t->d[i]);
    free(t->d);
    free(t);
}

InstanciaTSP *grafo_para_tsp(const GrafoHam *g) {
    InstanciaTSP *t = criar_instancia_tsp(g->n);
    if (!t) return NULL;
    for (int i = 0; i < g->n; i++)
        for (int j = 0; j < g->n; j++)
            if (i != j) t->d[i][j] = g->adj[i][j] ? 1 : 2;
    return t;
}

long custo_rota(const InstanciaTSP *t, const int *rota) {
    int n = t->n;
    long custo = 0;
    for (int i = 0; i + 1 < n; i++) custo += t->d[rota[i]][rota[i + 1]];
    if (n > 1) custo += t->d[rota[n - 1]][rota[0]];
    return custo;
}

int rota_valida(const InstanciaTSP *t, const int *rota) {
    int n = t->n;
    char *vista = calloc((size_t)(n > 0 ? n : 1), sizeof(char));
    int ok = 1;
    for (int i = 0; i < n && ok; i++) {
        if (rota[i] < 0 || rota[i] >= n || vista[rota[i]]) ok = 0;
        else vista[rota[i]] = 1;
    }
    free(vista);
    return ok;
}

/* ===================== Força bruta ===================== */

typedef struct {
    const InstanciaTSP *t;
    int *atual;
    int *melhor;
    char *usada;
    long melhor_custo;
    long long perms;
} CtxBF;

/* Gera as permutações das cidades 1..n-1 nas posições pos..n-1 (atual[0] = 0). */
static void permutar(CtxBF *c, int pos, long custo) {
    int n = c->t->n;

    if (pos == n) {   /* permutação completa: fecha o tour e compara */
        long total = custo + c->t->d[c->atual[n - 1]][c->atual[0]];
        c->perms++;
        if (total < c->melhor_custo) {
            c->melhor_custo = total;
            memcpy(c->melhor, c->atual, (size_t)n * sizeof(int));
        }
        return;
    }

    for (int v = 1; v < n; v++) {
        if (!c->usada[v]) {
            c->usada[v] = 1;
            c->atual[pos] = v;
            permutar(c, pos + 1, custo + c->t->d[c->atual[pos - 1]][v]);
            c->usada[v] = 0;
        }
    }
}

long tsp_forca_bruta(const InstanciaTSP *t, int *rota, long long *perms) {
    int n = t->n;
    if (n <= 0) {
        if (perms) *perms = 0;
        return 0;
    }

    CtxBF c;
    c.t = t;
    c.atual = calloc((size_t)n, sizeof(int));
    c.usada = calloc((size_t)n, sizeof(char));
    c.melhor = rota;
    c.melhor_custo = LONG_MAX;
    c.perms = 0;

    c.atual[0] = 0;   /* origem fixa: elimina as n rotações equivalentes de cada tour */
    c.usada[0] = 1;
    permutar(&c, 1, 0);

    if (perms) *perms = c.perms;
    long melhor = c.melhor_custo;
    free(c.atual);
    free(c.usada);
    return melhor;
}

/* ===================== Vizinho Mais Próximo ===================== */

long tsp_vizinho_mais_proximo(const InstanciaTSP *t, int *rota) {
    int n = t->n;
    if (n <= 0) return 0;

    char *visitada = calloc((size_t)n, sizeof(char));
    rota[0] = 0;
    visitada[0] = 1;

    for (int pos = 1; pos < n; pos++) {
        int atual = rota[pos - 1];
        int prox = -1;
        for (int v = 0; v < n; v++)
            if (!visitada[v] && (prox == -1 || t->d[atual][v] < t->d[atual][prox]))
                prox = v;
        rota[pos] = prox;
        visitada[prox] = 1;
    }

    free(visitada);
    return custo_rota(t, rota);
}

/* ===================== Inserção do Mais Próximo ===================== */

long tsp_insercao(const InstanciaTSP *t, int *rota) {
    int n = t->n;
    if (n <= 0) return 0;

    char *no_tour = calloc((size_t)n, sizeof(char));
    rota[0] = 0;
    no_tour[0] = 1;
    int tam = 1;

    if (n > 1) {   /* segundo vértice: o mais próximo da origem */
        int j = 1;
        for (int v = 2; v < n; v++)
            if (t->d[0][v] < t->d[0][j]) j = v;
        rota[1] = j;
        no_tour[j] = 1;
        tam = 2;
    }

    while (tam < n) {
        /* Cidade fora do tour mais próxima de alguma cidade do tour. */
        int k = -1, dmin = INT_MAX;
        for (int v = 0; v < n; v++) {
            if (no_tour[v]) continue;
            for (int p = 0; p < tam; p++) {
                if (t->d[rota[p]][v] < dmin) {
                    dmin = t->d[rota[p]][v];
                    k = v;
                }
            }
        }

        /* Melhor posição: entre rota[p] e rota[p+1] (cíclico), menor aumento de custo. */
        int pos = 0;
        long menor = LONG_MAX;
        for (int p = 0; p < tam; p++) {
            int a = rota[p];
            int b = rota[(p + 1) % tam];
            long aumento = (long)t->d[a][k] + t->d[k][b] - t->d[a][b];
            if (aumento < menor) {
                menor = aumento;
                pos = p;
            }
        }

        for (int p = tam; p > pos + 1; p--) rota[p] = rota[p - 1];   /* abre espaço */
        rota[pos + 1] = k;
        no_tour[k] = 1;
        tam++;
    }

    free(no_tour);
    return custo_rota(t, rota);
}
