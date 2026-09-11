#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

typedef struct No {
    int destino;
    struct No *prox;
} No;


typedef struct {
    int num_vertices;
    int dirigido;
    No **adj;
} GrafoLista;

GrafoLista *grafo_criar(int num_vertices, int dirigido);
void grafo_destruir(GrafoLista *g);
void grafo_add_aresta(GrafoLista *g, int origem, int destino);
void grafo_imprimir(const GrafoLista *g);

#endif