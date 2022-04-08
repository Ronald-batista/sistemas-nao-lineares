#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "mylib.h"



int main()
{
    int n_variaveis, max_iteracoes, i;
    double epsilon = 0.0000001;
    char string[] = "7*x1-log(x1)";
    char *expressao = string;
    double *resultado;
    double *aproximacao_inicial;
    max_iteracoes = 20;
    n_variaveis = 1;
    aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
    aproximacao_inicial[0] = 0.1;

    printf("\n------------- INICIANDO -------------\n");

    printf("\nNuméro de variaveis: %d\n", n_variaveis);
    printf("f(x) = %s\n", expressao);
    for (i = 0; i < n_variaveis; i++)
        printf("aproximação inicial [%d]: %1.14e\n",i, aproximacao_inicial[i]);

    printf("Epsilon: %1.14e\n", epsilon);
    printf("Max iterações: %d\n\n", max_iteracoes);

    resultado = newton(expressao, aproximacao_inicial, epsilon, max_iteracoes, n_variaveis);

     printf("\n------------- FINALIZANDO -------------\n");
    // for (i = 0; i < n_variaveis; i++){
    //     if (aproximacao_inicial[i] < 0 )
    //         aproximacao_inicial[i] = aproximacao_inicial[i] * (-1);
    //     printf("Aproximacao_inicial[%d] = %1.14e\n", i, aproximacao_inicial[i]);
    //     //printf("f(x) = %1.14e\n", norma_funcao(expressao, aproximacao_inicial));
    // }

    return 0;

    free(aproximacao_inicial);
}
