#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"
#include "newton.c"
#include <sys/stat.h>
#include "utils/Rosenbrock.c"
#include <likwid.h>


int main()
{
    LIKWID_MARKER_INIT;
    int n_variaveis, max_iteracoes, i, j;
    double epsilon;

    char expressao[1000000];
    char buf[1000000];
    double *aproximacao_inicial;
    double *value_funcao;
    double *resultado;
    int contador = 0;
    int marcador = 1;
    int iterador = 0;

    while (scanf("%s", buf) != EOF)
    {
        if (marcador == 1)
        { // numero de variveis
            n_variaveis = atoi(buf);
            printf("n_variaveis: %d\n", n_variaveis);
            aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
            marcador++;
            contador = n_variaveis;
        }
        else if (marcador == 2)
        { // expressao
            strcpy(expressao, buf);
            marcador++;
            printf("expressao: %s\n", expressao);
        }
        else if ((marcador == 3) && (contador > 0))
        { // aproximacao inicial
            aproximacao_inicial[iterador] = atof(buf);
            contador--;
            iterador++;
            if (contador == 0)
            {
                marcador++;
                printf("aproximacao inicial: [");
                for (i = 0; i < n_variaveis; i++)
                { // imprime as aproximacoes iniciais
                    printf("  %f  ", aproximacao_inicial[i]);
                }
                printf("]\n");
            }
        }
        else if (marcador == 4)
        { // epsilon
            epsilon = atof(buf);
            marcador++;
            printf("epsilon: %1.14e\n", epsilon);
        }
        else if (marcador == 5)
        { // maximo de iteracoes
            max_iteracoes = atoi(buf);
            marcador = 1;
            printf("max_iteracoes: %d\n", max_iteracoes);
            int DIMENSAO = n_variaveis;
            string_t markername;
            markername = markerName("Metodo_Newton:", DIMENSAO);
            LIKWID_MARKER_START(markername);
            value_funcao = malloc(max_iteracoes * sizeof(double));

            resultado = newton(expressao, aproximacao_inicial, epsilon, max_iteracoes, n_variaveis, value_funcao);
            // minimo_global(value_funcao, max_iteracoes);
            free(aproximacao_inicial);
            free(value_funcao);
            LIKWID_MARKER_STOP(markername);
            contador = 0;
            iterador = 0;
            printf("\n");
            memset(buf, 0, 1000000);
            memset(expressao, 0, 1000000);
        }
        else
        {
            printf("ERRO\n");
            break;
        }
    }
    LIKWID_MARKER_CLOSE;
    return 0;
}
