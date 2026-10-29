#ifndef AGM_H
#define AGM_H

#define INF 1000000000

/* Union-Find (conjuntos disjuntos). */
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

/* Aresta para o Kruskal (lista de arestas). */
typedef struct {
    int u, v, peso;
} Aresta;

/* Grafo não direcionado ponderado com lista de adjacência (usado pelo Prim). */
typedef struct NoAdj {
    int destino;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int n;         /* vértices 0 .. n-1 */
    NoAdj **adj;
} GrafoAGM;

/* ---- Union-Find ---- */
DSU *dsu_criar(int n);
void dsu_liberar(DSU *d);
int find(DSU *d, int x);          /* com compressão de caminho */
int unir(DSU *d, int x, int y);   /* por rank; retorna 1 se uniu, 0 se já estavam juntos */

/* ---- Grafo ---- */
GrafoAGM *criar_grafo_agm(int n);
void adicionar_aresta_agm(GrafoAGM *g, int u, int v, int peso); /* insere u-v e v-u */
void liberar_grafo_agm(GrafoAGM *g);

/*
 * Kruskal: O(E log E).
 * n = número de vértices, arestas[0..m-1] = arestas do grafo.
 * mst deve ter espaço para n-1 arestas; *qtd recebe quantas foram escolhidas.
 * Retorna o peso total. Se o grafo for desconexo, resulta em uma floresta
 * (*qtd < n-1).
 */
long kruskal(int n, const Aresta *arestas, int m, Aresta *mst, int *qtd);

/*
 * Prim com min-heap: O((V+E) log V).
 * Parte de 'origem'; pai[] e chave[] devem ter espaço para g->n inteiros.
 * pai[v] = pai de v na árvore (-1 para a origem ou vértice não alcançado);
 * chave[v] = peso da aresta que liga v ao pai (INF se não alcançado).
 * Retorna o peso total da árvore da componente da origem.
 */
long prim(GrafoAGM *g, int origem, int *pai, int *chave);

#endif  