#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

// --- Funções do Grafo ---
GrafoListaPonderado* criar_grafo(int num_vertices) {
    GrafoListaPonderado* g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = num_vertices;
    g->array = (ListaAdjacencia*) malloc(num_vertices * sizeof(ListaAdjacencia));
    for (int i = 0; i < num_vertices; ++i)
        g->array[i].cabeca = NULL;
    return g;
}

void adicionar_aresta(GrafoListaPonderado* g, int origem, int destino, int peso) {
    NoAresta* novo = (NoAresta*) malloc(sizeof(NoAresta));
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->array[origem].cabeca;
    g->array[origem].cabeca = novo;
}

void liberar_grafo(GrafoListaPonderado* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        NoAresta* atual = g->array[i].cabeca;
        while (atual) {
            NoAresta* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->array);
    free(g);
}

// --- Função: Exportar para .dot com alinhamento e linhas retas ---
void exportar_grafo_dot(GrafoListaPonderado *g, const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) {
        printf("Erro ao criar o arquivo %s\n", nome_arquivo);
        return;
    }

    fprintf(f, "graph G {\n");
    fprintf(f, "    rankdir=LR;\n"); 
    fprintf(f, "    splines=line;\n"); // Força o desenho de linhas retas
    fprintf(f, "    node [shape=circle];\n"); 
    
    // Forçar o alinhamento vertical exato da imagem
    fprintf(f, "    { rank=same; B; C; }\n");
    fprintf(f, "    { rank=same; D; E; }\n");

    for (int i = 0; i < g->num_vertices; i++) {
        NoAresta* atual = g->array[i].cabeca;
        while (atual != NULL) {
            if (i < atual->destino) { 
                char de = i + 'A';
                char para = atual->destino + 'A';
                fprintf(f, "    %c -- %c [label=\"%d\"];\n", de, para, atual->peso);
            }
            atual = atual->prox;
        }
    }
    
    fprintf(f, "}\n");
    fclose(f);
    printf("Arquivo '%s' gerado com sucesso!\n", nome_arquivo);
}

// --- Funções do Min-Heap ---
MinHeap* criar_min_heap(int capacidade) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->posicao = (int*) malloc(capacidade * sizeof(int));
    minHeap->capacidade = capacidade;
    minHeap->tamanho = 0;
    minHeap->dados = (NoHeap*) malloc(capacidade * sizeof(NoHeap));
    return minHeap;
}

void liberar_min_heap(MinHeap* minHeap) {
    free(minHeap->posicao);
    free(minHeap->dados);
    free(minHeap);
}

static void trocar_no_heap(MinHeap* minHeap, int i, int j) {
    NoHeap temp = minHeap->dados[i];
    minHeap->dados[i] = minHeap->dados[j];
    minHeap->dados[j] = temp;

    minHeap->posicao[minHeap->dados[i].vertice] = i;
    minHeap->posicao[minHeap->dados[j].vertice] = j;
}

static void min_heapify(MinHeap* minHeap, int idx) {
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < minHeap->tamanho && minHeap->dados[esq].distancia < minHeap->dados[menor].distancia)
        menor = esq;

    if (dir < minHeap->tamanho && minHeap->dados[dir].distancia < minHeap->dados[menor].distancia)
        menor = dir;

    if (menor != idx) {
        trocar_no_heap(minHeap, menor, idx);
        min_heapify(minHeap, menor);
    }
}

NoHeap extrair_minimo(MinHeap* minHeap) {
    if (minHeap->tamanho == 0)
        return (NoHeap){-1, -1};

    NoHeap raiz = minHeap->dados[0];
    NoHeap ultimo_no = minHeap->dados[minHeap->tamanho - 1];

    minHeap->dados[0] = ultimo_no;

    minHeap->posicao[raiz.vertice] = minHeap->tamanho - 1;
    minHeap->posicao[ultimo_no.vertice] = 0;

    minHeap->tamanho--;
    min_heapify(minHeap, 0);

    return raiz;
}

void diminuir_chave(MinHeap* minHeap, int vertice, int distancia) {
    int i = minHeap->posicao[vertice];
    minHeap->dados[i].distancia = distancia;

    while (i > 0 && minHeap->dados[i].distancia < minHeap->dados[(i - 1) / 2].distancia) {
        trocar_no_heap(minHeap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

// --- Funções do Dijkstra e Caminho ---
static int esta_no_min_heap(MinHeap *minHeap, int vertice) {
    return minHeap->posicao[vertice] < minHeap->tamanho;
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->num_vertices;
    MinHeap* minHeap = criar_min_heap(V);

    for (int v = 0; v < V; ++v) {
        dist[v] = INF;
        pred[v] = -1;
        minHeap->dados[v].vertice = v;
        minHeap->dados[v].distancia = dist[v];
        minHeap->posicao[v] = v;
    }

    minHeap->dados[origem].distancia = 0;
    dist[origem] = 0;
    diminuir_chave(minHeap, origem, dist[origem]);
    
    minHeap->tamanho = V;

    while (minHeap->tamanho > 0) {
        NoHeap min_no = extrair_minimo(minHeap);
        int u = min_no.vertice;

        NoAresta* atual = g->array[u].cabeca;
        while (atual != NULL) {
            int v = atual->destino;
            
            if (esta_no_min_heap(minHeap, v) && dist[u] != INF && atual->peso + dist[u] < dist[v]) {
                dist[v] = dist[u] + atual->peso;
                pred[v] = u;
                diminuir_chave(minHeap, v, dist[v]);
            }
            atual = atual->prox;
        }
    }
    
    liberar_min_heap(minHeap);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (pred[destino] == -1 && destino != origem) {
        printf("Nao ha caminho entre %c e %c\n", origem + 'A', destino + 'A');
        return;
    }
    
    int caminho[1000];
    int cont = 0;
    int atual = destino;
    
    while (atual != -1) {
        caminho[cont++] = atual;
        atual = pred[atual];
    }
    
    printf("Caminho de %c para %c: ", origem + 'A', destino + 'A');
    for (int i = cont - 1; i >= 0; i--) {
        printf("%c", caminho[i] + 'A');
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}