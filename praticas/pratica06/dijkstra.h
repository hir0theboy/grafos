#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#define INF INT_MAX

// --- Estruturas Auxiliares do Grafo ---
typedef struct NoAresta {
    int destino;
    int peso;
    struct NoAresta *prox;
} NoAresta;

typedef struct {
    NoAresta *cabeca;
} ListaAdjacencia;

typedef struct {
    int num_vertices;
    ListaAdjacencia *array;
} GrafoListaPonderado;

// --- Estruturas Obrigatórias do Min-Heap ---
typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

// --- Protótipos das Funções do Grafo ---
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado* g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado* g);
void exportar_grafo_dot(GrafoListaPonderado *g, const char *nome_arquivo);

// --- Protótipos Obrigatórios ---
MinHeap* criar_min_heap(int capacidade);
NoHeap extrair_minimo(MinHeap* minHeap);
void diminuir_chave(MinHeap* minHeap, int vertice, int distancia);
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

// --- Funções Auxiliares do Min-Heap ---
void liberar_min_heap(MinHeap* minHeap);

#endif