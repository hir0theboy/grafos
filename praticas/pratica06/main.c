#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

int main() {
    int V = 6; // 6 vértices: A(0), B(1), C(2), D(3), E(4), F(5)
    GrafoListaPonderado* g = criar_grafo(V);

    // Grafo não-direcionado da imagem
    adicionar_aresta(g, 0, 1, 4); adicionar_aresta(g, 1, 0, 4); // A-B
    adicionar_aresta(g, 0, 2, 2); adicionar_aresta(g, 2, 0, 2); // A-C
    adicionar_aresta(g, 1, 2, 1); adicionar_aresta(g, 2, 1, 1); // B-C
    adicionar_aresta(g, 1, 3, 5); adicionar_aresta(g, 3, 1, 5); // B-D
    adicionar_aresta(g, 1, 4, 8); adicionar_aresta(g, 4, 1, 8); // B-E
    adicionar_aresta(g, 2, 3, 4); adicionar_aresta(g, 3, 2, 4); // C-D
    adicionar_aresta(g, 2, 4, 10); adicionar_aresta(g, 4, 2, 10); // C-E
    adicionar_aresta(g, 3, 4, 2); adicionar_aresta(g, 4, 3, 2); // D-E
    adicionar_aresta(g, 3, 5, 1); adicionar_aresta(g, 5, 3, 1); // D-F
    adicionar_aresta(g, 4, 5, 3); adicionar_aresta(g, 5, 4, 3); // E-F

    // Gerando o arquivo .dot para visualização no Graphviz
    exportar_grafo_dot(g, "grafo.dot");

    int dist[V];
    int pred[V];
    int origem = 0; // A(0)

    dijkstra(g, origem, dist, pred);

    printf("\n--- Distancias a partir da origem %c ---\n", origem + 'A');
    for (int i = 0; i < V; ++i) {
        if (dist[i] == INF)
            printf("Vertice %c: Inacessivel\n", i + 'A');
        else
            printf("Vertice %c: %d\n", i + 'A', dist[i]);
    }

    printf("\n--- Reconstrucao dos Caminhos ---\n");
    for (int i = 0; i < V; ++i) {
        if (i != origem) {
            reconstruir_caminho(origem, i, pred);
        }
    }

    liberar_grafo(g);
    return 0;
}