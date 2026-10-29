#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#define INF 1000000000

/* Nó da lista de adjacência (aresta u -> destino com peso). */
typedef struct NoAresta {
    int destino;
    int peso;
    struct NoAresta *prox;
} NoAresta;

/* Grafo direcionado ponderado com lista de adjacência. */
typedef struct {
    int n;           /* número de vértices (0 .. n-1) */
    NoAresta **adj;  /* adj[u] = lista de arestas saindo de u */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int n);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

/*
 * Bellman-Ford a partir de 'origem'.
 * dist[] e pred[] devem ter espaço para g->n inteiros.
 * Retorna 1 se não há ciclo negativo alcançável, 0 caso contrário.
 */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/*
 * Deve ser chamada após bellman_ford retornar 0.
 * Retrocede n vezes por pred[] para cair dentro do ciclo, depois
 * o percorre e imprime. Retorna 1 se encontrou e imprimiu o ciclo.
 */
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred);

#endif