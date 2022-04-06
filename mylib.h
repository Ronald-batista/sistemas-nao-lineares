#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"

#define ERRO_OPERACAO_MATEMATICA_INVALIDA 1.000000000000000

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

void gerar_matriz_jacobiana(void **matriz_jacobiana, char *expressao, int n_variaveis, double *tempo_derivadas)
{
    int length; /* Length of above buffer. */
    int i, j;
    void *funcao, *funcao_derivada_prim; /* Evaluators for function and function derivativchar **names; */
    int contador;                        /* Number of function variables. */
    char **nomes_variaveis;              /* Function variables names. */
    double tempo;

    // printf("\n ------------ GERAR MATRIZ JACOBIANA  ------------\n");
    // criar expressão valida para a libmatheval
    funcao = evaluator_create(expressao);
    // printf("Função criada, f(x) = %s\n", evaluator_get_string(funcao));

    // Mostra as variaveis que a função possui
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);
    // printf(" ");
    // for (i = 0; i < contador; i++)
    //  printf("%s", nomes_variaveis[i]);
    // printf("\n");
    *tempo_derivadas = timestamp();
    // percorre a matriz jacobiana, atribuindo equações a cada elemento da matriz
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < n_variaveis; j++)
        {
            funcao_derivada_prim = evaluator_derivative(funcao, nomes_variaveis[j]);
            matriz_jacobiana[j] = funcao_derivada_prim;
            // printf(" f’(x) = %s\n", evaluator_get_string(matriz_jacobiana[j]));
        }
    }
    *tempo_derivadas = timestamp() - *tempo_derivadas;
}

double norma_funcao(char *expressao, double *aproximacao_inicial)
{
    void *funcao;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;
    // printf("\n ------------ NORMA FUNCAO  ------------\n");

    // Criar expressão valida para a libmatheval
    funcao = evaluator_create(expressao);
    // preenche o vetor de nomes das variaveis e contador de variaveis
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);
    // printf("f(x) = %s\n", evaluator_get_string(funcao));
    //printf("aproximação inicial: %1.14e\n", aproximacao_inicial[0]);

    double norma = evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial);
    // printf("NORMA = %1.14e\n", norma );
    if (norma < 0)
    { // mmodulo do resultado se necessario
        norma = norma * (-1);
        return norma;
    }
    else if (norma > 0)
        return norma;
    // evaluator_destroy(funcao);
    return ERRO_OPERACAO_MATEMATICA_INVALIDA;
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

double calcula_expressao(void *expressao, double *aproximacao_inicial)
{
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;
    void *funcao;
    funcao = evaluator_create(expressao);
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);

    // printf("f(x) = %s\n", evaluator_get_string(funcao));
    // printf("Contador = %d\n", contador);
    // printf("Nomes Variaveis = %s\n", nomes_variaveis[0]);
    // printf("Aproximação inicial = %1.14e \n", aproximacao_inicial[0]);
    double resultado = evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial);
    if (resultado < 0 || resultado > 0)
    {
        return resultado;
    }

    return ERRO_OPERACAO_MATEMATICA_INVALIDA;
}

double **calcular_matriz_jacobiana(void **matriz_jacobiana, double **matriz_jacobiana_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i, j;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;

    // printf("\n ------------ CALCULAR MATRIZ JACOBIANA  ------------\n");
    for (i = 0; i < 1; i++)
        for (j = 0; j < n_variaveis; j++)
        {
            // preenche o vetor de nomes das variaveis e contador de variaveis
            evaluator_get_variables(matriz_jacobiana[j], &nomes_variaveis, &contador);

            //    printf("f(x) = %s\n", evaluator_get_string(matriz_jacobiana[j]));
            //    printf("Contador = %d\n", contador);
            //    printf("Nomes Variaveis = %s\n", nomes_variaveis[j]);
            //    printf("Aproximação inicial = %1.14e \n", aproximacao_inicial[j]);

            // calcula
            matriz_jacobiana_calc[i][j] = evaluator_evaluate(matriz_jacobiana[j], contador, nomes_variaveis, aproximacao_inicial);
        }
    return matriz_jacobiana_calc;
}

void calcula_delta(double **matriz_jacobiana_calc, double **delta, void *expressao, double *aproximacao_inicial, int n_variaveis, double *tempo_sistemas_lineares)
{
    int i, j;
    *tempo_sistemas_lineares = timestamp();
    // printf("\n ------------ CALCULANDO O DELTA-----------\n");
    for (i = 0; i < 1; i++)
        for (j = 0; j < n_variaveis; j++)
        {
            double valor = calcula_expressao(expressao, aproximacao_inicial);
            //  printf("VALOR = %1.14e  \n", valor);
            if (valor == ERRO_OPERACAO_MATEMATICA_INVALIDA)
            {
               // printf("ERRO - CRITÉRIO DE PARADA: log negativo! Critério de parada");
                delta[j][i] = valor;
                *tempo_sistemas_lineares = timestamp() - *tempo_sistemas_lineares;
                return;
            }
            // printf("VALOR = %1.14e  \n", valor);
            // printf("MATRIZ_JACOBIANA_CALC: %lf \n", matriz_jacobiana_calc[i][j]);
            delta[j][i] = (-1 * valor) / matriz_jacobiana_calc[i][j];
            //  printf("DELTA = %1.14e \n", delta[j][i]);
        }
    *tempo_sistemas_lineares = timestamp() - *tempo_sistemas_lineares;
    // printf("\n ------------ FIM DELTA----------\n");
}

void proxima_aproximacao(double *aproximacao_inicial, double **delta, int n_variaveis)
{
    int i, j;

    // printf("\n ------------ PROXIMA APROXIMAÇÃO-----------\n");
    for (i = 0; i < 1; i++)
        for (j = 0; j < n_variaveis; j++)
        {
            // printf("aproximação inicial = %1.14e \n", aproximacao_inicial[j]);
            // printf("delta = %1.14e \n", delta[j][i]);
            // if (aproximacao_inicial[j] < 0)
            //     aproximacao_inicial[j] = aproximacao_inicial[j] * (-1);
            if (delta[i][j] == ERRO_OPERACAO_MATEMATICA_INVALIDA)
                return;
            aproximacao_inicial[j] = aproximacao_inicial[j] + delta[j][i];
            // printf("NOVA aproximação inicial = %1.14e\n", aproximacao_inicial[j]);
        }
}

double norma_delta(double **delta, int n_variaveis)
{

    double max, aux;
    int i, j;
    // printf("\n ------------ NORMA DELTA-----------\n");

    max = delta[0][0];
    aux = 0;
    for (i = 0; i < n_variaveis; i++)
        for (j = 1; j < 1; j++)
        {
            if (delta[i][j] == ERRO_OPERACAO_MATEMATICA_INVALIDA)
                return ERRO_OPERACAO_MATEMATICA_INVALIDA;
            if (delta[i][j] < 0)
                aux = -1 * delta[i][j];

            if (delta[i][j] > max)
                max = delta[i][j];
            else if (aux > max)
                max = aux;
        }

    if (max < 0)
        max = -1 * max;

    // printf("NORMA DELTA: %1.14e \n", max);
    return max;
}

/*
    Método de NEWTON
*/

double *newton(char *expressao, double *aproximacao_inicial, double epsilon, int max_iteracoes, int n_variaveis)
{
    int i;
    void **matriz_jacobiana;
    double **matriz_jacobiana_calc;
    double **delta;
    double norma_f;
    double tempo, tempo_derivadas, tempo_sistemas_lineares;
    // printf("------------------INICIANDO MÉTODO DE NEWTON------------------\n");

    // newtin = malloc(max_iteracoes-1 * sizeof(double));

    // alocar matriz jacobiana do tipo void => matriz linha
    matriz_jacobiana = malloc(1 * sizeof(void *));
    for (i = 0; i < 1; i++)
    {
        matriz_jacobiana[i] = malloc(n_variaveis * sizeof(void *));
    }

    // alocar matriz jacobiana calculada do tipo double  => matriz linha
    matriz_jacobiana_calc = malloc(1 * sizeof(double *));
    for (i = 0; i < 1; i++)
    {
        matriz_jacobiana_calc[i] = malloc(n_variaveis * sizeof(double));
    }

    // alocar matriz delta => matriz coluna
    delta = malloc(n_variaveis * sizeof(double));
    for (i = 0; i < n_variaveis; i++)
    {
        delta[i] = malloc(1 * sizeof(double));
    }
    
    gerar_matriz_jacobiana(matriz_jacobiana, expressao, n_variaveis, &tempo_derivadas);
    
    printf("#Iteração \t| Newton Padrão \t| \n");
    for (i = 0; i < max_iteracoes; i++)
    {
        printf("%d\t\t| ", i);

        // Verifica criterio de parada - Norma da função na iteração atual é menor que o epsilon?
        norma_f = norma_funcao(expressao, aproximacao_inicial);
        // newtin[i] = norma_f;
       // printf("\n NORMA: %1.14e\n", norma_f);  
        if (norma_f == ERRO_OPERACAO_MATEMATICA_INVALIDA)
        {
            printf("Ocorreu uma operação matematica inválida\t|\t\t\n"); 
            tempo = timestamp();
            printf("\nTEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }

        printf("%1.14e\t|\t\t\n", norma_f);
        if (norma_f < epsilon)
        {
            tempo = timestamp();
            printf("\nTEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }

        //  printf("%1.14e | \n", norma_f);
        // resolve o sistema linear para encontrar o delta. J(X^i)*DELTA^i = -F(X^i)
        // eliminacaoGauss(matriz_jacobiana, delta, calcula_expressao(expressao, aproximacao_inicial, i));
        matriz_jacobiana_calc = calcular_matriz_jacobiana(matriz_jacobiana, matriz_jacobiana_calc, aproximacao_inicial, n_variaveis);
        calcula_delta(matriz_jacobiana_calc, delta, expressao, aproximacao_inicial, n_variaveis, &tempo_sistemas_lineares);
        // proxima aproximação
        proxima_aproximacao(aproximacao_inicial, delta, n_variaveis);

        double normaDelta = norma_delta(delta, n_variaveis);
        // printf("DELTA TESTE: %1.14e \n", normaDelta);
        if (normaDelta < epsilon || normaDelta == ERRO_OPERACAO_MATEMATICA_INVALIDA)
        {
            tempo = timestamp();
            printf("\nTEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }
    }

    // libera a memória da matriz_jacobiana
    for (i = 0; i < 1; i++)
        free(matriz_jacobiana[i]);
    free(matriz_jacobiana);

    // libera a memória da matriz_jacobiana_calc
    for (i = 0; i < 1; i++)
        free(matriz_jacobiana_calc[i]);
    free(matriz_jacobiana_calc);

    // libera a memória da matriz_jacobiana_calc
    for (i = 0; i < 1; i++)
        free(delta[i]);
    free(delta);
}