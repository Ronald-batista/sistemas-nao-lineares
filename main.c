#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "mylib.h"

int main()
{
    int n_variaveis, max_iteracoes, i;
    double epsilon = 0.0000001;
    char string[] = "9*x1-4*log(x1-7)";
    char *expressao = string;
    double *resultado;
    double *aproximacao_inicial;
    aproximacao_inicial = malloc(1 * sizeof(double *));
    aproximacao_inicial[0] =7.01;
    max_iteracoes = 20;
    n_variaveis = 1;

    // printf("Digite o numero de variaveis que função possui: \n");
    // scanf("%d", &n_variaveis);

    // aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
    // // aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
    // printf("Digite a aproximação inicial: \n");
    // for (i = 0; i < n_variaveis; i++)
    //     scanf("%lf", &aproximacao_inicial[i]);

    // printf("Digite a tolerancia (EPSILON) para o criterio de parada: \n");
    // scanf("%lf", &epsilon);

    // printf("Digite a quantidade maxima de iterações permitidas: \n");
    // scanf("%d", &max_iteracoes);

    printf("\nNuméro de variaveis: %d\n", n_variaveis);
    printf("f(x) = %s\n", expressao);
    for (i = 0; i < n_variaveis; i++)
        printf("aproximação inicial: %1.14e\n", aproximacao_inicial[i]);

    printf("Epsilon: %1.14e\n", epsilon);
    printf("Max iterações: %d\n", max_iteracoes);

    resultado = newton(expressao, aproximacao_inicial, epsilon, max_iteracoes, n_variaveis);

    printf("FINALIZANDO ----\n");
    for (i = 0; i < n_variaveis; i++)
        printf("Aproximacao_inicial[%d] = %lf", i, aproximacao_inicial[i]);

    return 0;

    free(aproximacao_inicial);
}
