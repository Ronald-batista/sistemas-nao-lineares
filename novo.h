#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"

/*
Imprime uma matriz coluna que seja do tipo void
*/
void imprime_matriz_coluna_void(void **matriz, int linhas, int colunas)
{
    int i, j;
    printf("IMPRIME MATRIZ VOID");
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            printf("\n%s\t| ", evaluator_get_string(matriz[i]));
    printf("\n");
}

/*
Cria vetor gradiente => matriz coluna com a derivada primeira da função em relação a cada icognita
*/
void vetor_gradiente(void **gradiente, void *expressao)
{

    int i, j;
    void *funcao;
    char **icognitas;
    int contador;
    funcao = evaluator_create(expressao);
    evaluator_get_variables(funcao, &icognitas, &contador);

    for (i = 0; i < contador; i++)
        for (j = 0; j < 1; j++)
        {
            gradiente[i] = evaluator_derivative(funcao, icognitas[i]);
        }
    imprime_matriz_coluna_void(gradiente, contador, 1);
}

/*
Gera uma matriz Hessiana NxN => contem a derivada segunda de cada elemento do vetor gradiente em relação a cada icognita da função
 */
void gera_matriz_hessiana(void ***hessiana, void **gradiente, int n_variaveis)
{
    int i, j;
    void *funcao;
    char **icognitas;
    int contador;
    printf("-----------GERARANDO MATRIZ HESSIANA-----------\n");
    for (i = 0; i < n_variaveis; i++)
        for (j = 0; j < n_variaveis; j++)
        {
            evaluator_get_variables(gradiente[i], &icognitas, &contador);
            funcao = evaluator_derivative(gradiente[i], icognitas[j]);
            printf("iteração i = %d |  f(x) = %s\n", i, evaluator_get_string(funcao));
            hessiana[i][j] = evaluator_derivative(funcao, icognitas[j]);
        }

    // printar matriz hessiana
    for (i = 0; i < n_variaveis; i++)
        for (j = 0; j < n_variaveis; j++)
            printf("%s\t| ", evaluator_get_string(hessiana[i][j]));
    printf("\n");
    printf("-----------MATRIZ HESSIANA GERADA COM SUCESSO-----------\n");
}

void calcula_matriz_hessiana(void ***matriz_hessiana, double **matriz_hessiana_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i, j;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;

    printf("-----------CALCULANDO MATRIZ HESSIANA-----------\n");
    for (i = 0; i < n_variaveis; i++)
    {
        for (j = 0; j < n_variaveis; j++)
        {
            // preenche o vetor de nomes das variaveis e contador de variaveis
            evaluator_get_variables(matriz_hessiana[i][j], &nomes_variaveis, &contador);

            // calcula
            matriz_hessiana_calc[i][j] = evaluator_evaluate(matriz_hessiana[i][j], contador, nomes_variaveis, aproximacao_inicial);
        }
    }
      // printar matriz hessiana
    for (i = 0; i < n_variaveis; i++)
        for (j = 0; j < n_variaveis; j++)
            printf("%1.14e\t| ", matriz_hessiana_calc[i][j]);

    printf("-----------MATRIZ HESSIANA CALCULADA COM SUCESSO-----------\n");
}

/* Seja um S.L. de ordem 'n'
 */
// void eliminacaoGauss(double **matriz, double *b, uint n)
// {
//     /* para cada linha a partir da primeira */
//     for (int i = 0; i < n; ++i)
//     {
//         uint iPivo = encontraMax(matriz, i);
//         if (i != iPivo)
//             trocaLinha(matriz, b, i, iPivo);
//         for (int k = i + 1; k < n; ++k)
//         {
//             double m = matriz[k][i] / matriz[i][i];
//             matriz[k][i] = 0.0;
//             for (int j = i + 1; j < n; ++j)
//                 matriz[k][j] -= matriz[i][j] * m;
//             b[k] -= b[i] * m;
//         }
//     }
// }
