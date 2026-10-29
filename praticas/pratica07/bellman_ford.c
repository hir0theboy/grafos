#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

GrafoListaPonderado *criar_grafo(int n) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)n, sizeof(NoAresta *));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    NoAresta *no = malloc(sizeof(NoAresta));
    if (!no) return;
    no->destino = v;
    no->peso = peso;
    no->prox = g->adj[u];
    g->adj[u] = no;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (!g) return;
    for (int u = 0; u < g->n; u++) {
        NoAresta *a = g->adj[u];
        while (a) {
            NoAresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(g->adj);
    free(g);
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->n;

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[origem] = 0;

    /* n-1 iterações de relaxação de todas as arestas */
    for (int i = 1; i <= n - 1; i++) {
        int mudou = 0;
        for (int u = 0; u < n; u++) {
            if (dist[u] == INF) continue;
            for (NoAresta *a = g->adj[u]; a; a = a->prox) {
                if (dist[u] + a->peso < dist[a->destino]) {
                    dist[a->destino] = dist[u] + a->peso;
                    pred[a->destino] = u;
                    mudou = 1;
                }
            }
        }
        if (!mudou) return 1; /* convergiu antes: não há ciclo negativo */
    }

    /* n-ésima iteração: se ainda relaxa, existe ciclo negativo alcançável */
    for (int u = 0; u < n; u++) {
        if (dist[u] == INF) continue;
        for (NoAresta *a = g->adj[u]; a; a = a->prox) {
            if (dist[u] + a->peso < dist[a->destino]) {
                pred[a->destino] = u;
                return 0;
            }
        }
    }

    return 1;
}

/* Menor peso entre as arestas paralelas u -> v. */
static int peso_aresta(GrafoListaPonderado *g, int u, int v) {
    int melhor = INF;
    for (NoAresta *a = g->adj[u]; a; a = a->prox)
        if (a->destino == v && a->peso < melhor) melhor = a->peso;
    return melhor;
}

int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred) {
    int n = g->n;
    int x = -1;

    /* Acha um vértice que ainda pode ser relaxado (na n-ésima iteração). */
    for (int u = 0; u < n && x == -1; u++) {
        if (dist[u] == INF) continue;
        for (NoAresta *a = g->adj[u]; a; a = a->prox) {
            if (dist[u] + a->peso < dist[a->destino]) {
                pred[a->destino] = u;
                x = a->destino;
                break;
            }
        }
    }
    if (x == -1) {
        printf("Nenhum ciclo negativo encontrado.\n");
        return 0;
    }

    /* Retrocede n vezes por pred[] para garantir que x está dentro do ciclo. */
    for (int i = 0; i < n; i++) {
        if (pred[x] == -1) {
            printf("Falha ao reconstruir o ciclo.\n");
            return 0;
        }
        x = pred[x];
    }

    /* Percorre o ciclo a partir de x guardando os vértices (ordem reversa). */
    int *ciclo = malloc((size_t)(n + 1) * sizeof(int));
    if (!ciclo) return 0;
    int tam = 0;
    int v = x;
    do {
        ciclo[tam++] = v;
        v = pred[v];
    } while (v != x && tam <= n);

    printf("Ciclo negativo: ");
    long soma = 0;
    for (int i = tam - 1; i >= 0; i--) {
        int atual = ciclo[i];
        int prox = ciclo[(i + tam - 1) % tam];
        printf("%d -> ", atual);
        soma += peso_aresta(g, atual, prox);
    }
    printf("%d\n", ciclo[tam - 1]);
    printf("Peso total do ciclo: %ld\n", soma);

    free(ciclo);
    return 1;
}