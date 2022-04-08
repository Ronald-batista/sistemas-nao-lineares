#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"
#include "novo.h"

int main()
{
    int n_variaveis, max_iteracoes, i,j;
    double epsilon = 0.0000001;
    char string[] = "-log(1-x1-x2)-log(x1)-log(x2)";
    char *expressao = string;
    double *resultado;
    double *aproximacao_inicial;
    max_iteracoes = 20;
    n_variaveis = 2;
    aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
    aproximacao_inicial[0] = 0.85;
     aproximacao_inicial[1] = 0.05;


    // aloca matriz gradiente[n_variaveis][1]  ==> matriz coluna (matriz que possui apenas uma coluna e n linhas)
    void **gradiente;
    gradiente = malloc(n_variaveis * sizeof(void *));
    for (i = 0; i < n_variaveis; i++)
    {
        gradiente[i] = malloc(1 * sizeof(void *));
    }

    // aloca matriz hessiana[n_variaveis][n_variaveis]  ==> matriz quadrada
    void ***hessiana;
    hessiana = malloc(n_variaveis * sizeof(void *));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana[i] = malloc(n_variaveis * sizeof(void *));
    }

     // aloca matriz hessiana calculada[n_variaveis][n_variaveis]  ==> calcula a matriz nos pontos de aproximação da iteração
    double **hessiana_calc;
    hessiana_calc = malloc(n_variaveis * sizeof(double ));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = malloc(n_variaveis * sizeof(double ));
    }

    // aloca delta => matriz coluna
    double **delta;
    delta = malloc(n_variaveis * sizeof(double));
    for (i = 0; i < n_variaveis; i++)
    {
        delta[i] = malloc(1 * sizeof(double));
    }
    

    vetor_gradiente(gradiente, expressao);
    gera_matriz_hessiana(hessiana, gradiente, n_variaveis);
    calcula_matriz_hessiana(hessiana, hessiana_calc, aproximacao_inicial, n_variaveis);

}