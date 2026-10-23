#include <stdio.h>
#include "grafo_lista.h"

int main() {
    GrafoLista *grafo = criar_grafo(5);

    adicionar_aresta(grafo, 0, 1);
        adicionar_aresta(grafo, 0, 2);
            adicionar_aresta(grafo, 1, 3);
                adicionar_aresta(grafo, 2, 3);
                    adicionar_aresta(grafo, 2, 4);
                        adicionar_aresta(grafo, 3, 4);
                            adicionar_aresta(grafo, 4, 5);

    for(int i; i < grafo->num_vertices; i++)
    {
        printf("%i: -> ", i+1);
        No *no = grafo->lista[i];
        while (no != NULL) {
            printf("%i->", no->vertice+1);
            no = no ->proximo;
        }
        printf("NULL\n");
    }


    return 0;
}