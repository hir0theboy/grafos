#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

#define NUM_INST 5            /* instâncias aleatórias por tamanho */
#define DIST_MAX 100          /* distâncias aleatórias em 1..DIST_MAX */
#define TEMPO_MIN_MEDICAO (CLOCKS_PER_SEC / 20)   /* repete até medir ~50 ms */

/* ===================== Utilidades ===================== */

static void imprimir_sequencia(const int *seq, int n, int fechar) {
    for (int i = 0; i < n; i++) printf("%d%s", seq[i], (i + 1 < n || fechar) ? " -> " : "");
    if (fechar) printf("%d", seq[0]);
}

static long long fatorial(int k) {
    long long f = 1;
    for (int i = 2; i <= k; i++) f *= i;
    return f;
}

static GrafoHam *montar_grafo(int n, const int (*arestas)[2], int m) {
    GrafoHam *g = criar_grafo_ham(n);
    for (int i = 0; i < m; i++) adicionar_aresta_ham(g, arestas[i][0], arestas[i][1]);
    return g;
}

/* ===================== Parte 1: Caminho/Ciclo Hamiltoniano + redução ===================== */

static void testar_grafo(const char *nome, int n, const int (*arestas)[2], int m) {
    GrafoHam *g = montar_grafo(n, arestas, m);
    int *seq = malloc((size_t)n * sizeof(int));

    printf("--- %s (n = %d, m = %d) ---\n", nome, n, m);

    int tem_caminho = encontrar_caminho_hamiltoniano_bt(g, seq);
    printf("  Caminho Hamiltoniano: ");
    if (tem_caminho) {
        printf("sim: ");
        imprimir_sequencia(seq, n, 0);
        printf("\n");
    } else {
        printf("nao\n");
    }

    int tem_ciclo = encontrar_ciclo_hamiltoniano_bt(g, seq);
    printf("  Ciclo Hamiltoniano:   ");
    if (tem_ciclo) {
        printf("sim: ");
        imprimir_sequencia(seq, n, 1);
        printf("\n");
    } else {
        printf("nao\n");
    }

    /* Redução para TSP: o ciclo existe se e somente se o custo ótimo for n. */
    InstanciaTSP *t = grafo_para_tsp(g);
    int *rota = malloc((size_t)n * sizeof(int));
    long long perms;
    long otimo = tsp_forca_bruta(t, rota, &perms);
    int ciclo_pela_reducao = (otimo == n);
    printf("  Reducao TSP: custo otimo = %ld (n = %d) => ciclo %s; %lld permutacoes ((n-1)! = %lld)\n",
           otimo, n, ciclo_pela_reducao ? "existe" : "nao existe", perms, fatorial(n - 1));
    printf("  Consistente com o backtracking? %s\n\n",
           ciclo_pela_reducao == tem_ciclo ? "sim" : "NAO (erro!)");

    free(rota);
    free(seq);
    liberar_instancia_tsp(t);
    liberar_grafo_ham(g);
}

static void parte_hamiltoniano(void) {
    printf("=== Parte 1: Caminho e Ciclo Hamiltoniano (backtracking) + reducao para TSP ===\n");

    const int pentagono[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}};
    testar_grafo("Pentagono com corda", 5, pentagono, 6);

    const int caminho[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    testar_grafo("Caminho 0-1-2-3-4", 5, caminho, 4);

    const int estrela[][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}};
    testar_grafo("Estrela (centro 0)", 5, estrela, 4);

    const int petersen[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0},   /* pentagono externo */
        {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9},   /* raios */
        {5, 7}, {7, 9}, {9, 6}, {6, 8}, {8, 5}    /* pentagrama interno */
    };
    testar_grafo("Grafo de Petersen", 10, petersen, 15);
}

/* ===================== Parte 2: TSP (experimento) ===================== */

typedef long (*FuncTSP)(const InstanciaTSP *, int *);

static long bf_sem_contagem(const InstanciaTSP *t, int *rota) {
    return tsp_forca_bruta(t, rota, NULL);
}

static InstanciaTSP *instancia_aleatoria(int n) {
    InstanciaTSP *t = criar_instancia_tsp(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int d = 1 + rand() % DIST_MAX;
            t->d[i][j] = d;
            t->d[j][i] = d;
        }
    }
    return t;
}

/*
 * Roda f sobre todas as instâncias, repetindo até acumular tempo suficiente
 * para a resolução do relógio. Na primeira passada guarda os custos e valida
 * cada rota (permutação das cidades e custo consistente). Retorna µs por execução.
 */
static double medir(FuncTSP f, InstanciaTSP **inst, int num, long *custos, int *rota, int *ok) {
    clock_t ini = clock();
    long passadas = 0;
    do {
        for (int k = 0; k < num; k++) {
            long c = f(inst[k], rota);
            if (passadas == 0) {
                custos[k] = c;
                if (!rota_valida(inst[k], rota) || custo_rota(inst[k], rota) != c) *ok = 0;
            }
        }
        passadas++;
    } while ((clock() - ini) < TEMPO_MIN_MEDICAO);
    return 1e6 * (double)(clock() - ini) / CLOCKS_PER_SEC / ((double)passadas * num);
}

static void parte_tsp(void) {
    int tamanhos[] = {5, 8, 10, 12};
    int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));

    printf("=== Parte 2: TSP, %d instancias aleatorias por tamanho (distancias 1..%d) ===\n",
           NUM_INST, DIST_MAX);
    printf("%3s | %14s %11s %11s | %9s %9s %9s | %9s %9s\n",
           "n", "Forca bruta us", "Vizinho us", "Insercao us",
           "Custo FB", "Custo VMP", "Custo INS", "gap VMP", "gap INS");
    printf("----+-------------------------------------------+-------------------------------+-------------------\n");

    srand(42);
    for (int s = 0; s < qtd; s++) {
        int n = tamanhos[s];
        InstanciaTSP *inst[NUM_INST];
        for (int k = 0; k < NUM_INST; k++) inst[k] = instancia_aleatoria(n);

        int *rota = malloc((size_t)n * sizeof(int));
        long c_fb[NUM_INST], c_vmp[NUM_INST], c_ins[NUM_INST];
        int ok = 1;

        double us_fb = medir(bf_sem_contagem, inst, NUM_INST, c_fb, rota, &ok);
        double us_vmp = medir(tsp_vizinho_mais_proximo, inst, NUM_INST, c_vmp, rota, &ok);
        double us_ins = medir(tsp_insercao, inst, NUM_INST, c_ins, rota, &ok);

        double soma_fb = 0, soma_vmp = 0, soma_ins = 0, gap_vmp = 0, gap_ins = 0;
        for (int k = 0; k < NUM_INST; k++) {
            soma_fb += c_fb[k];
            soma_vmp += c_vmp[k];
            soma_ins += c_ins[k];
            gap_vmp += 100.0 * (c_vmp[k] - c_fb[k]) / c_fb[k];
            gap_ins += 100.0 * (c_ins[k] - c_fb[k]) / c_fb[k];
            if (c_vmp[k] < c_fb[k] || c_ins[k] < c_fb[k]) ok = 0;   /* heuristica nao pode vencer o otimo */
        }

        printf("%3d | %14.1f %11.2f %11.2f | %9.1f %9.1f %9.1f | %8.1f%% %8.1f%%%s\n",
               n, us_fb, us_vmp, us_ins,
               soma_fb / NUM_INST, soma_vmp / NUM_INST, soma_ins / NUM_INST,
               gap_vmp / NUM_INST, gap_ins / NUM_INST, ok ? "" : "  (ERRO!)");

        for (int k = 0; k < NUM_INST; k++) liberar_instancia_tsp(inst[k]);
        free(rota);
    }
    printf("\n(gap = excesso medio de custo em relacao a forca bruta, que e o otimo)\n");
}

int main(void) {
    parte_hamiltoniano();
    parte_tsp();
    return 0;
}
