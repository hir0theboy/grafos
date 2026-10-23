    #include <stdio.h>

    #include "grafo_matriz.h"
    #include "grafo_lista.h"

    void exibir_matriz(GrafoMatriz *grafo) {
        for (int i = 0; i < grafo->n; i++) {
            for (int j = 0; j < grafo->n; j++) {
                printf("%3d", grafo->adj[i][j]);
            }

            printf("\n");
        }
    }

    void exibir_lista(GrafoLista *grafo) {
        for (int i = 0; i < grafo->n; i++) {
            No *atual = grafo->adj[i];

            printf("%d:", i);

            while (atual != NULL) {
                printf(" %d", atual->destino);
                atual = atual->prox;
            }

            printf("\n");
        }
    }

    int main() {

        printf("=== GRAFO COM MATRIZ DE ADJACENCIA ===\n\n");

        GrafoMatriz *grafo_matriz = criar_grafo_matriz(8);

        if (grafo_matriz == NULL) {
            printf("Erro ao criar o grafo por matriz.\n");
            return 1;
        }

        inserir_aresta_matriz(grafo_matriz, 0, 1);
        inserir_aresta_matriz(grafo_matriz, 0, 2);
        inserir_aresta_matriz(grafo_matriz, 0, 3);

        inserir_aresta_matriz(grafo_matriz, 1, 4);
        inserir_aresta_matriz(grafo_matriz, 1, 5);

        inserir_aresta_matriz(grafo_matriz, 2, 3);
        inserir_aresta_matriz(grafo_matriz, 2, 6);

        inserir_aresta_matriz(grafo_matriz, 3, 6);

        inserir_aresta_matriz(grafo_matriz, 7, 4);
        inserir_aresta_matriz(grafo_matriz, 7, 5);
        inserir_aresta_matriz(grafo_matriz, 7, 6);

        printf("Matriz de adjacencia:\n");

        exibir_matriz(grafo_matriz);

        printf("\nGrau do vertice 0: %d\n",
            grau_matriz(grafo_matriz, 0));

        printf("Grau do vertice 1: %d\n",
            grau_matriz(grafo_matriz, 1));

        printf("0 e 1 sao adjacentes? %s\n",
            sao_adjacentes_matriz(grafo_matriz, 0, 1)
            ? "Sim"
            : "Nao");

        printf("0 e 7 sao adjacentes? %s\n",
            sao_adjacentes_matriz(grafo_matriz, 0, 7)
            ? "Sim"
            : "Nao");

        printf("\nRemovendo a aresta 0-1...\n");

        remover_aresta_matriz(grafo_matriz, 0, 1);

        exibir_matriz(grafo_matriz);

        liberar_grafo_matriz(grafo_matriz);


        printf("\n=== GRAFO COM LISTA DE ADJACENCIA ===\n\n");

        GrafoLista *grafo_lista = criar_grafo_lista(8);

        if (grafo_lista == NULL) {
            printf("Erro ao criar o grafo por lista.\n");
            return 1;
        }

        inserir_aresta_lista(grafo_lista, 0, 1);
        inserir_aresta_lista(grafo_lista, 0, 2);
        inserir_aresta_lista(grafo_lista, 0, 3);

        inserir_aresta_lista(grafo_lista, 1, 4);
        inserir_aresta_lista(grafo_lista, 1, 5);

        inserir_aresta_lista(grafo_lista, 2, 3);
        inserir_aresta_lista(grafo_lista, 2, 6);

        inserir_aresta_lista(grafo_lista, 3, 6);

        inserir_aresta_lista(grafo_lista, 7, 4);
        inserir_aresta_lista(grafo_lista, 7, 5);
        inserir_aresta_lista(grafo_lista, 7, 6);

        printf("Lista de adjacencia:\n");

        exibir_lista(grafo_lista);

        printf("\nGrau do vertice 0: %d\n",
            grau_lista(grafo_lista, 0));

        printf("Grau do vertice 1: %d\n",
            grau_lista(grafo_lista, 1));

        printf("0 e 1 sao adjacentes? %s\n",
            sao_adjacentes_lista(grafo_lista, 0, 1)
            ? "Sim"
            : "Nao");

        printf("0 e 7 sao adjacentes? %s\n",
            sao_adjacentes_lista(grafo_lista, 0, 7)
            ? "Sim"
            : "Nao");

        printf("\nRemovendo a aresta 0-1...\n");

        remover_aresta_lista(grafo_lista, 0, 1);

        exibir_lista(grafo_lista);

        liberar_grafo_lista(grafo_lista);

        return 0;
    }