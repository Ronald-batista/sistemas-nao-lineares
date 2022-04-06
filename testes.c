#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "testes.h"

int main()
{
    int i;
    // int n_variaveis, max_iteracoes;
    // double  aproximacao_inicial, epsilon;
    // char *expressao = "7*x1-log(x1)\0";

    char string[] = "7*x1-log(x1)\0";
    int n_variaveis = 1;
    char *expressao = string;
    void **matriz_jacobiana;
    double **matriz_jacobiana_calc;
    double *aproximacao_inicial;
    aproximacao_inicial = malloc(1 * sizeof(double *));
    aproximacao_inicial[0] = 0.0000001;

    norma_funcao(expressao, aproximacao_inicial);

    // alocar matriz jacobiana do tipo void => matriz linha
    matriz_jacobiana = malloc(1 * sizeof(void *));
    for (i = 0; i < 1; i++)
    {
        matriz_jacobiana[i] = malloc(n_variaveis * sizeof(void));
    }

    // alocar matriz jacobiana calculada do tipo double  => matriz linha
    matriz_jacobiana_calc = malloc(1 * sizeof(double *));
    for (i = 0; i < 1; i++)
    {
        matriz_jacobiana_calc[i] = malloc(n_variaveis * sizeof(double));
    }

    // alocar matriz delta => matriz coluna
    double **delta;
    for (i = 0; i < n_variaveis; i++)
    {
        delta[i] = malloc(1 * sizeof(double));
    }

    printf("\n");
    gerar_matriz_jacobiana(matriz_jacobiana, expressao, n_variaveis);
    imprime_matriz_void(matriz_jacobiana, 1, n_variaveis);
    // teste(matriz_jacobiana, aproximacao_inicial, n_variaveis);
    calcular_matriz_jacobiana(matriz_jacobiana, matriz_jacobiana_calc, aproximacao_inicial, n_variaveis);
    imprime_matriz_double(matriz_jacobiana_calc, 1, n_variaveis);

    // calcula o valor da expressão no ponto da aproximação
    printf("\n ------------ CALCULAR EXPRESSAO ------------\n");
    double valor = calcula_expressao(expressao, aproximacao_inicial);
    printf("valor = %lf\n", valor);


    // Encontrar matriz delta J(X^i)*DELTA^i = -F(X^i)
    calcula_delta(matriz_jacobiana_calc, delta, expressao, aproximacao_inicial, n_variaveis);

    proxima_aproximacao(aproximacao_inicial, delta, n_variaveis);

    norma_delta(delta, n_variaveis);
    // eliminação de gauss para J(x) * delta = aproximacao_inicial;
    // eliminacaoGauss(matriz_jacobiana, delta, valor);
}
