    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include "cobertura.h"

    #define NUM_GRAFOS 20      /* grafos aleatórios por tamanho */
    #define DENSIDADE 40       /* probabilidade (%) de cada aresta existir */
    #define REPS_BT 20         /* repetições para medir o backtracking */
    #define REPS_GULOSO 2000   /* repetições para medir o guloso (é bem mais rápido) */

    static void imprimir_cobertura(const char *nome, const int *sol, int n, int tam) {
        printf("  %-14s tam = %d, vertices = {", nome, tam);
        int primeiro = 1;
        for (int v = 0; v < n; v++) {
            if (sol[v]) {
                printf("%s%d", primeiro ? "" : ", ", v);
                primeiro = 0;
            }
        }
        printf("}\n");
    }

    static GrafoCobertura *grafo_aleatorio(int n) {
        GrafoCobertura *g = criar_grafo_cob(n);
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                if (rand() % 100 < DENSIDADE) adicionar_aresta_cob(g, u, v);
        return g;
    }

    static void demonstracao(void) {
        printf("=== Demonstracao: grafo pequeno (6 vertices) ===\n");
        GrafoCobertura *g = criar_grafo_cob(6);
        int arestas[][2] = {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 5}};
        for (int i = 0; i < 8; i++) adicionar_aresta_cob(g, arestas[i][0], arestas[i][1]);

        int sol_bt[6], sol_gu[6];
        long long nos;
        int tam_bt = cobertura_minima_bt(g, sol_bt, &nos);
        int tam_gu = cobertura_gulosa(g, sol_gu);

        imprimir_cobertura("Backtracking:", sol_bt, 6, tam_bt);
        imprimir_cobertura("Guloso:", sol_gu, 6, tam_gu);
        printf("  Backtracking valido? %s | Guloso valido? %s | nos visitados: %lld\n\n",
            eh_cobertura(g, sol_bt) ? "sim" : "NAO",
            eh_cobertura(g, sol_gu) ? "sim" : "NAO", nos);
        liberar_grafo_cob(g);
    }

    static void experimento(void) {
        int tamanhos[] = {5, 8, 10, 12, 15};
        int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));

        printf("=== Experimento: %d grafos aleatorios por tamanho, densidade %d%% ===\n",
            NUM_GRAFOS, DENSIDADE);
        printf("%3s | %12s | %12s | %10s | %8s | %8s | %s\n",
            "n", "BT (us)", "Guloso (us)", "nos BT", "otimo", "guloso", "guloso otimo");
        printf("----+--------------+--------------+------------+----------+----------+-------------\n");

        srand(42);
        for (int t = 0; t < qtd; t++) {
            int n = tamanhos[t];
            GrafoCobertura *grafos[NUM_GRAFOS];
            for (int k = 0; k < NUM_GRAFOS; k++) grafos[k] = grafo_aleatorio(n);

            int *sol_bt = malloc((size_t)n * sizeof(int));
            int *sol_gu = malloc((size_t)n * sizeof(int));
            long long nos_total = 0;
            long soma_bt = 0, soma_gu = 0;
            int iguais = 0, invalidos = 0;
            volatile int sink = 0;

            /* Tamanhos e validade (uma execução por grafo). */
            for (int k = 0; k < NUM_GRAFOS; k++) {
                long long nos;
                int tb = cobertura_minima_bt(grafos[k], sol_bt, &nos);
                int tg = cobertura_gulosa(grafos[k], sol_gu);
                nos_total += nos;
                soma_bt += tb;
                soma_gu += tg;
                if (tb == tg) iguais++;
                if (!eh_cobertura(grafos[k], sol_bt) || !eh_cobertura(grafos[k], sol_gu) || tg < tb)
                    invalidos++;
            }

            /* Tempo: média por execução, repetindo para superar a resolução do relógio. */
            clock_t ini = clock();
            for (int r = 0; r < REPS_BT; r++)
                for (int k = 0; k < NUM_GRAFOS; k++) sink += cobertura_minima_bt(grafos[k], sol_bt, NULL);
            double us_bt = 1e6 * (double)(clock() - ini) / CLOCKS_PER_SEC / (REPS_BT * NUM_GRAFOS);

            ini = clock();
            for (int r = 0; r < REPS_GULOSO; r++)
                for (int k = 0; k < NUM_GRAFOS; k++) sink += cobertura_gulosa(grafos[k], sol_gu);
            double us_gu = 1e6 * (double)(clock() - ini) / CLOCKS_PER_SEC / (REPS_GULOSO * NUM_GRAFOS);

            printf("%3d | %12.2f | %12.2f | %10.1f | %8.2f | %8.2f | %2d de %d%s\n",
                n, us_bt, us_gu, (double)nos_total / NUM_GRAFOS,
                (double)soma_bt / NUM_GRAFOS, (double)soma_gu / NUM_GRAFOS,
                iguais, NUM_GRAFOS, invalidos ? "  (ERRO!)" : "");

            for (int k = 0; k < NUM_GRAFOS; k++) liberar_grafo_cob(grafos[k]);
            free(sol_bt);
            free(sol_gu);
            (void)sink;
        }
    }

    int main(void) {
        demonstracao();
        experimento();
        return 0;
    }