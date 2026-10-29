#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agm.h"

/* ===================== Union-Find ===================== */

DSU *dsu_criar(int n) {
    DSU *d = malloc(sizeof(DSU));
    if (!d) return NULL;
    d->n = n;
    d->pai = malloc((size_t)n * sizeof(int));
    d->rank = calloc((size_t)n, sizeof(int));
    if (!d->pai || !d->rank) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }
    for (int i = 0; i < n; i++) d->pai[i] = i;
    return d;
}

void dsu_liberar(DSU *d) {
    if (!d) return;
    free(d->pai);
    free(d->rank);
    free(d);
}

/* Compressão de caminho: todo nó visitado passa a apontar direto para a raiz. */
int find(DSU *d, int x) {
    if (d->pai[x] != x)
        d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

/* União por rank: a árvore mais baixa é pendurada na mais alta. */
int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0;

    if (d->rank[rx] < d->rank[ry]) {
        int t = rx;
        rx = ry;
        ry = t;
    }
    d->pai[ry] = rx;
    if (d->rank[rx] == d->rank[ry]) d->rank[rx]++;
    return 1;
}

/* ===================== Grafo ===================== */

GrafoAGM *criar_grafo_agm(int n) {
    GrafoAGM *g = malloc(sizeof(GrafoAGM));
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)n, sizeof(NoAdj *));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    return g;
}

static void inserir_adj(GrafoAGM *g, int u, int v, int peso) {
    NoAdj *no = malloc(sizeof(NoAdj));
    if (!no) return;
    no->destino = v;
    no->peso = peso;
    no->prox = g->adj[u];
    g->adj[u] = no;
}

void adicionar_aresta_agm(GrafoAGM *g, int u, int v, int peso) {
    inserir_adj(g, u, v, peso);
    inserir_adj(g, v, u, peso);
}

void liberar_grafo_agm(GrafoAGM *g) {
    if (!g) return;
    for (int u = 0; u < g->n; u++) {
        NoAdj *a = g->adj[u];
        while (a) {
            NoAdj *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(g->adj);
    free(g);
}

/* ===================== Kruskal ===================== */

static int cmp_aresta(const void *a, const void *b) {
    const Aresta *x = a;
    const Aresta *y = b;
    return (x->peso > y->peso) - (x->peso < y->peso);
}

long kruskal(int n, const Aresta *arestas, int m, Aresta *mst, int *qtd) {
    *qtd = 0;
    if (n <= 0) return 0;

    /* Ordena uma cópia para não alterar o vetor do chamador: O(E log E). */
    Aresta *ord = malloc((size_t)(m > 0 ? m : 1) * sizeof(Aresta));
    DSU *d = dsu_criar(n);
    if (!ord || !d) {
        free(ord);
        dsu_liberar(d);
        return -1;
    }
    memcpy(ord, arestas, (size_t)m * sizeof(Aresta));
    qsort(ord, (size_t)m, sizeof(Aresta), cmp_aresta);

    long total = 0;
    int k = 0;
    for (int i = 0; i < m && k < n - 1; i++) {
        if (unir(d, ord[i].u, ord[i].v)) {   /* só aceita se não formar ciclo */
            mst[k++] = ord[i];
            total += ord[i].peso;
        }
    }
    *qtd = k;

    free(ord);
    dsu_liberar(d);
    return total;
}

/* ===================== Prim com Min-Heap ===================== */

/* Heap de vértices ordenado por chave[vértice]; pos[v] = índice de v no heap
 * (ou -1 se v já saiu), o que permite diminuir a chave em O(log V). */
typedef struct {
    int *vert;
    int *pos;
    int *chave;
    int tam;
} MinHeap;

static void heap_trocar(MinHeap *h, int i, int j) {
    int t = h->vert[i];
    h->vert[i] = h->vert[j];
    h->vert[j] = t;
    h->pos[h->vert[i]] = i;
    h->pos[h->vert[j]] = j;
}

static void heap_subir(MinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->chave[h->vert[i]] < h->chave[h->vert[p]]) {
            heap_trocar(h, i, p);
            i = p;
        } else {
            break;
        }
    }
}

static void heap_descer(MinHeap *h, int i) {
    for (;;) {
        int l = 2 * i + 1;
        int r = l + 1;
        int menor = i;
        if (l < h->tam && h->chave[h->vert[l]] < h->chave[h->vert[menor]]) menor = l;
        if (r < h->tam && h->chave[h->vert[r]] < h->chave[h->vert[menor]]) menor = r;
        if (menor == i) break;
        heap_trocar(h, i, menor);
        i = menor;
    }
}

static int heap_extrair_min(MinHeap *h) {
    int v = h->vert[0];
    h->tam--;
    if (h->tam > 0) {
        h->vert[0] = h->vert[h->tam];
        h->pos[h->vert[0]] = 0;
        heap_descer(h, 0);
    }
    h->pos[v] = -1;
    return v;
}

long prim(GrafoAGM *g, int origem, int *pai, int *chave) {
    int n = g->n;
    MinHeap h;
    h.vert = malloc((size_t)n * sizeof(int));
    h.pos = malloc((size_t)n * sizeof(int));
    h.chave = chave;
    h.tam = n;
    if (!h.vert || !h.pos) {
        free(h.vert);
        free(h.pos);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        chave[i] = INF;
        pai[i] = -1;
    }
    chave[origem] = 0;

    /* Origem na raiz (chave 0); os demais têm INF, então o heap já é válido. */
    h.vert[0] = origem;
    h.pos[origem] = 0;
    int idx = 1;
    for (int v = 0; v < n; v++) {
        if (v == origem) continue;
        h.vert[idx] = v;
        h.pos[v] = idx++;
    }

    long total = 0;
    while (h.tam > 0) {
        int u = heap_extrair_min(&h);
        if (chave[u] == INF) break;   /* o resto é inalcançável (grafo desconexo) */
        total += chave[u];

        for (NoAdj *a = g->adj[u]; a; a = a->prox) {
            int v = a->destino;
            if (h.pos[v] != -1 && a->peso < chave[v]) {
                chave[v] = a->peso;   /* atualização de chave */
                pai[v] = u;
                heap_subir(&h, h.pos[v]);
            }
        }
    }

    free(h.vert);
    free(h.pos);
    return total;
}