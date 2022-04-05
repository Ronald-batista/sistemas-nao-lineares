#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
/* Size of input buffer. */
#define BUFFER_SIZE 256

// void alocar_matriz(double **matriz_jacobiana, int linhas, int colunas)
// {
//     int i;
//     matriz_jacobiana = malloc(linhas * sizeof(double *));
//     for (i = 0; i < 10; i++)
//     {
//         matriz_jacobiana[i] = malloc(colunas * sizeof(double));
//     }
// }

void preenche_matriz(void **matriz_jacobiana, int linhas, int colunas)
{
    int i, j;

    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            matriz_jacobiana[i] = 0;
        }
    }
}

void imprime_matriz_void(void **matriz_jacobiana, int linhas, int colunas)
{
    int i, j;

    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            printf("%s | ", evaluator_get_string(matriz_jacobiana[j]));
        }
        printf("\n");
    }
}

void imprime_matriz_double(double **matriz_jacobiana_calc, int linhas, int colunas)
{
    int i, j;

    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            printf("%lf | ", matriz_jacobiana_calc[i][j]);
        }
        printf("\n");
    }
}

void gerar_matriz_jacobiana(void **matriz_jacobiana, char *expressao, int n_variaveis)
{
    int length; /* Length of above buffer. */
    int i, j;
    void *funcao, *funcao_derivada_prim; /* Evaluators for function and function derivativchar **names; */
    int contador;                        /* Number of function variables. */
    char **nomes_variaveis;              /* Function variables names. */

    
    printf("\n ------------ GERAR MATRIZ JACOBIANA  ------------\n");
    // criar expressão valida para a libmatheval
    funcao = evaluator_create(expressao);
    printf("Função criada, f(x) = %s\n", evaluator_get_string(funcao));

    // Mostra as variaveis que a função possui
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);
    printf(" ");
    for (i = 0; i < contador; i++)
        printf("%s", nomes_variaveis[i]);
    printf("\n");

    // percorre a matriz jacobiana, atribuindo equações a cada elemento da matriz
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < n_variaveis; j++)
        {
            funcao_derivada_prim = evaluator_derivative(funcao, nomes_variaveis[j]);
            matriz_jacobiana[i] = funcao_derivada_prim;
            printf(" f’(x) = %s\n", evaluator_get_string(matriz_jacobiana[j]));
        }
    }
    //evaluator_destroy (funcao);
    //evaluator_destroy (funcao_derivada_prim);
    //free(nomes_variaveis);
}

double norma(char *expressao, double *aproximacao_inicial)
{
    void *funcao;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;

    // Criar expressão valida para a libmatheval
    funcao = evaluator_create(expressao);

    // preenche o vetor de nomes das variaveis e contador de variaveis
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);

    double norma = evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial);
    if (norma < 0)
    { // mmodulo do resultado se necessario
        norma = norma * (-1);
    }
    printf("\n ------------ CALCULAR NORMA  ------------\n");
    printf("f(x) = %s\n", evaluator_get_string(funcao));
    printf("Contador = %d\n", contador);
    printf("Nomes Variaveis = %s\n", nomes_variaveis[0]);
    printf("Aproximação inicial = %lf\n", aproximacao_inicial[0]);
    printf("NORMA DA FUNÇÃO: %lf \n", evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial));

    evaluator_destroy (funcao);
    return norma;
}

/* Seja um S.L. de ordem 'n'
 */
// void eliminacaoGauss(double **matriz_jacobiana, double *b, uint n)
// {
//     /* para cada linha a partir da primeira */
//     for (int i = 0; i < n; ++i)
//     {
//         uint iPivo = encontraMax(matriz_jacobiana, i);
//         if (i != iPivo)
//             trocaLinha(matriz_jacobiana, b, i, iPivo);
//         for (int k = i + 1; k < n; ++k)
//         {
//             double m = matriz_jacobiana[k][i] / matriz_jacobiana[i][i];
//             matriz_jacobiana[k][i] = 0.0;
//             for (int j = i + 1; j < n; ++j)
//                 matriz_jacobiana[k][j] -= matriz_jacobiana[i][j] * m;
//             b[k] -= b[i] * m;
//         }
//     }
// }

// uint calcula_expressao(void *expressao, double *aproximacao_inicial)
// {
//     int contador; /* Numero de variaveis na função*/
//     char **nomes_variaveis;
//     evaluator_get_variables(expressao, &nomes_variaveis, &contador);

//     printf("f(x) = %s\n", evaluator_get_string(expressao));
//     printf("Contador = %d\n", contador);
//     printf("Nomes Variaveis = %s\n", nomes_variaveis[0]);
//     printf("Aproximação inicial = %lf\n", aproximacao_inicial[0]);
//     //uint resultado = evaluator_evaluate(expressao, contador, nomes_variaveis, aproximacao_inicial);
//     uint resultado = 2;

//     return resultado;
// }

double **calcular_matriz_jacobiana(void **matriz_jacobiana, double **matriz_jacobiana_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i, j;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;

    printf("\n ------------ CALCULAR MATRIZ JACOBIANA  ------------\n");
    for (i = 0; i < 1; i++)
        for (j = 0; j < n_variaveis; j++)
        {
            // preenche o vetor de nomes das variaveis e contador de variaveis
            evaluator_get_variables(matriz_jacobiana[j], &nomes_variaveis, &contador);
            
            printf("f(x) = %s\n", evaluator_get_string(matriz_jacobiana[j]));
            printf("Contador = %d\n", contador);
            printf("Nomes Variaveis = %s\n", nomes_variaveis[j]);
            printf("Aproximação inicial = %lf\n", aproximacao_inicial[j]);

            // calcula
            matriz_jacobiana_calc[i][j] = evaluator_evaluate(matriz_jacobiana[j], contador, nomes_variaveis, aproximacao_inicial);
        }
    return matriz_jacobiana_calc;
}

// void teste(void **matriz_jacobiana, double *aproximacao_inicial, int n_variaveis)
// {
//     int i, j;
//     int contador; /* Numero de variaveis na função*/
//     char **nomes_variaveis;

//     printf ("-------------__TESTE_-------------------\n");
//     printf("TESTE: %s\n", evaluator_get_string(matriz_jacobiana[0]));
//     evaluator_get_variables(matriz_jacobiana[0], &nomes_variaveis, &contador);
//      printf("Contador = %d\n", contador);
//      printf("Nomes Variaveis = %s\n", nomes_variaveis[0]);

// }