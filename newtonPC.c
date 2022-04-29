#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"
#include "newtonPC.h"
#include <sys/stat.h>

int main()
{
    int n_variaveis, max_iteracoes, i, j;
    double epsilon;

    char expressao[1000];
    char buf[1000];
    double *aproximacao_inicial;
    double *value_funcao;
    double *final;
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
            value_funcao = malloc(max_iteracoes * sizeof(double));

            final = newton(expressao, aproximacao_inicial, epsilon, max_iteracoes, n_variaveis, value_funcao);
            // minimo_global(value_funcao, max_iteracoes);

            contador = 0;
            iterador = 0;
            printf("\n");

            free(aproximacao_inicial);
            free(value_funcao);
            memset(buf, 0, 1000);
            memset(expressao, 0, 1000);
        }
        else
        {
            printf("ERRO\n");
            break;
        }
    }

    return 0;
}
