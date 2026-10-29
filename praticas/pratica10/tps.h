#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* Instância do TSP simétrico: d[i][j] = distância entre as cidades i e j. */
typedef struct {
    int n;
    int **d;
} InstanciaTSP;

InstanciaTSP *criar_instancia_tsp(int n);   /* d[i][i] = 0, demais = 0 até o preenchimento */
void liberar_instancia_tsp(InstanciaTSP *t);

/*
 * Redução Ciclo Hamiltoniano -> TSP: peso 1 para arestas existentes e peso 2
 * para as inexistentes. O grafo tem ciclo Hamiltoniano se e somente se o
 * tour ótimo do TSP custa exatamente n.
 */
InstanciaTSP *grafo_para_tsp(const GrafoHam *g);

/* Custo do tour fechado rota[0] -> ... -> rota[n-1] -> rota[0]. */
long custo_rota(const InstanciaTSP *t, const int *rota);

/* Retorna 1 se rota[] é uma permutação das n cidades. */
int rota_valida(const InstanciaTSP *t, const int *rota);

/*
 * TSP por força bruta: fixa a cidade 0 como origem e avalia as (n-1)!
 * permutações das demais. rota[] recebe o melhor tour (n posições, com
 * rota[0] = 0). perms (opcional, pode ser NULL) recebe quantas permutações
 * foram avaliadas. Retorna o custo ótimo.
 */
long tsp_forca_bruta(const InstanciaTSP *t, int *rota, long long *perms);

/* Heurística construtiva do Vizinho Mais Próximo, partindo da cidade 0. */
long tsp_vizinho_mais_proximo(const InstanciaTSP *t, int *rota);

/*
 * Heurística de Inserção do Mais Próximo: começa com a cidade 0 e a mais
 * próxima dela; a cada passo escolhe a cidade fora do tour mais próxima de
 * alguma cidade do tour e a insere na posição de menor aumento de custo.
 */
long tsp_insercao(const InstanciaTSP *t, int *rota);

#endif
