#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"

/*
Cria vetor gradiente => matriz coluna com a derivada primeira da função em relação a cada icognita
*/
void gera_vetor_gradiente(void **gradiente, void *expressao, double *tempo_derivadas)
{

    int i, j;
    void *funcao;
    char **icognitas;
    int contador;
    double tempo;
    funcao = evaluator_create(expressao);
    evaluator_get_variables(funcao, &icognitas, &contador);

    tempo = timestamp();
    for (i = 0; i < contador; i++)
    {
        for (j = 0; j < 1; j++)
        {
            gradiente[i] = evaluator_derivative(funcao, icognitas[i]);
        }
    }
    tempo = timestamp() - tempo; // tempo gasto para o calculo das derivadas
    *tempo_derivadas = tempo + *tempo_derivadas;
    }

/*
Gera uma matriz Hessiana NxN => contem a derivada segunda de cada elemento do vetor gradiente em relação a cada icognita da função
 */
void gera_matriz_hessiana(void ***hessiana, void **gradiente, int n_variaveis, double *tempo_derivadas)
{
    int i, j;
    void *funcao;
    char **icognitas;
    int contador;
    double tempo;

    tempo = timestamp();
    for (i = 0; i < n_variaveis; i++)
    {
        for (j = 0; j < n_variaveis; j++)
        {
            evaluator_get_variables(gradiente[i], &icognitas, &contador);
            hessiana[i][j] = evaluator_derivative(gradiente[i], icognitas[j]);
        }
    }
    tempo = timestamp() - tempo; // tempo gasto para o calculo das derivadas
    *tempo_derivadas = *tempo_derivadas + tempo;
}

void calcula_matriz_hessiana(void ***matriz_hessiana, double **matriz_hessiana_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i, j;
    int contador; /* Numero de variaveis na função*/
    char **nomes_variaveis;

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
}

/* calcula o vetor gradiente nos pontos de aproximação da iteração
 */

void calcula_vetor_gradiente(void **gradiente, double *gradiente_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i, j;
    void *funcao;
    char **icognitas;
    int contador;

    for (i = 0; i < n_variaveis; i++)
        for (j = 0; j < 1; j++)
        {
            evaluator_get_variables(gradiente[i], &icognitas, &contador);
            gradiente_calc[i] = evaluator_evaluate(gradiente[i], contador, icognitas, aproximacao_inicial);
        }
}

/*  Encontra o elemento de maior valor de uma matriz na linha i e retorna a sua posição
 */
uint encontraMax(double **matriz, int linha, int coluna)
{
    double max = matriz[0][0];
    uint indice_pivo = 0;
    double modulo;
    int i, j;
    for (i = 0; i < linha; i++)
    {
        for (j = coluna; j < coluna + 1; j++)
        {
            if (matriz[i][j] < 0)
                modulo = matriz[i][j] * (-1);
            else
                modulo = matriz[i][j];

            if (modulo > max)
            {
                max = modulo;
                indice_pivo = i;
            }
        }
    }
    return indice_pivo;
}

/*  troca a linha i da matriz, pela linha ipivo. Ocorre a mesma troca no vetor dos elementos independentes.
 */
void trocaLinha(double **matriz, double *vetor_independente, int i, uint ipivo, int max_col)
{
    double **matriz_temporaria;
    int linha, coluna;
    double temp;

    // troca linhas matriz
    for (linha = i; linha < i + 1; linha++)
    {
        for (coluna = 0; coluna < max_col; coluna++)
        {
            temp = matriz[linha][coluna];
            matriz[linha][coluna] = matriz[ipivo][coluna];
            matriz[ipivo][coluna] = temp;
        }
    }

    // troca do vetor independente
    temp = vetor_independente[i];
    vetor_independente[i] = vetor_independente[ipivo];
    vetor_independente[ipivo] = temp;
}

/* Seja um S.L. de ordem 'n' realiza a eliminação de gauss na matriz com os termos independentes
 */
void eliminacaoGauss(double **matriz, double *vetor_independente, uint n)
{
    int i, j;
    /* para cada linha a partir da primeira */
    for (int i = 0; i < n; ++i)
    {
        uint iPivo = encontraMax(matriz, n, i);
        if (i != iPivo)
            trocaLinha(matriz, vetor_independente, i, iPivo, n);
        for (int k = i + 1; k < n; ++k)
        {
            double m = matriz[k][i] / matriz[i][i];
            matriz[k][i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                matriz[k][j] -= matriz[i][j] * m;
            vetor_independente[k] -= vetor_independente[i] * m;
        }
    }
}

/* Seja um S.L. triangula de orden n, encontra as suas raizes e armazena no vetor delta

 */
void retrossubs(double **matriz, double *vetor_independente, double *delta, uint n)
{
    int i, j;
    // printf("\n-----------RETROSUBSTITUIÇÃO-----------\n");
    //  caso base de só haver uma expressao
    if (n == 1)
    {
        delta[0] = vetor_independente[0] / matriz[0][0];
        return;
    }

    for (int i = n - 1; i >= 0; --i)
    {
        delta[i] = vetor_independente[i];
        for (int j = i + 1; j < n; ++j)
            delta[i] -= matriz[i][j] * delta[j];
        delta[i] /= matriz[i][i];
    }
}

/* Calcula a norma do vetor gradientes nos pontos x^i
 */
double norma_grad(void **gradiente, double *gradiente_calc, double *aproximacao_inicial, int n_variaveis)
{
    int i;
    double norma, temp;
    calcula_vetor_gradiente(gradiente, gradiente_calc, aproximacao_inicial, n_variaveis);

    // encontra a norma do vetor gradiente_calc
    norma = gradiente_calc[0];
    for (i = 0; i < n_variaveis; i++)
    {
        temp = gradiente_calc[i];
        if (temp < 0)
        {
            temp = temp * (-1);
        }
        if (temp > norma)
            norma = temp;
    }
    return norma;
}

/* Calcula a norma de f(x) nos pontos x^i
 */
double funcao_apresentacao(void *expressao, double *aproximacao_inicial, int n_variaveis)
{
    int i;
    double valor, temp;
    char **icognitas;
    int contador;
    void *f;
    // encontra a norma do gradiente_calc
    f = evaluator_create(expressao);
    evaluator_get_variables(f, &icognitas, &contador);
    valor = evaluator_evaluate(f, contador, icognitas, aproximacao_inicial);
    if (valor > 0 || valor < 0)
        return valor;
    else
        return 1.00000000000000;
}

/* Calcula o delta ==> resolve o sistema linear H(X^(i))𝚫^(i) = -𝛻f (X^(i))
 */
void calcula_delta(void ***hessiana, double **hessiana_calc, void **gradiente, double *gradiente_calc, double *delta, double *aproximacao_inicial, int n_variaveis, double *tempo_sistemas_lineares)
{
    int i;
    double tempo;
    calcula_matriz_hessiana(hessiana, hessiana_calc, aproximacao_inicial, n_variaveis);
    // calcula_vetor_gradiente(gradiente, gradiente_calc, aproximacao_inicial, n_variaveis);

    // multiplica o gradiente calculado por -1
    for (i = 0; i < n_variaveis; i++)
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
        // printf("gradiente_calc = %lf\n", gradiente_calc[i]);
    }

    tempo = timestamp();

    eliminacaoGauss(hessiana_calc, gradiente_calc, n_variaveis);
    retrossubs(hessiana_calc, gradiente_calc, delta, n_variaveis);

    tempo = timestamp() - tempo; // tempo para resolução do sistema linear (eliminação de gauss + retrosubstituição)
    *tempo_sistemas_lineares = *tempo_sistemas_lineares + tempo;
}

/* Gera os proximos pontos de aproximação
 */
void proxima_aproximacao(double *delta, double *aproximacao_inicial, int n_variaveis)
{
    int i;
    for (i = 0; i < n_variaveis; i++)
    {
        aproximacao_inicial[i] = aproximacao_inicial[i] + delta[i];
    }
}

/*  Encontra a norma do vetor delta
 */
double norma_delta(double *delta, int n_variaveis)
{
    int i;
    double norma, temp;
    norma = delta[0];
    for (i = 0; i < n_variaveis; i++)
    {
        temp = delta[0];
        if (temp < 0)
            temp = temp * (-1);
        if (temp > norma)
            norma = temp;
    }
    return norma;
}

/* Imprime o valor minimo de um vetor double
 */
void minimo_global(double *norma_funcao, int max_iteracoes)
{
    double min;
    int i;
    min = norma_funcao[0];
    for (i = 0; i < max_iteracoes; i++)
    {
        if (norma_funcao[i] == 0.0)
        {
            printf("MINIMO GLOBAL = %1.14e\n", min);
            break;
        }
        if (min > norma_funcao[i])
        {
            min = norma_funcao[i];
        }
        if (i == max_iteracoes - 1)
            printf("MINIMO GLOBAL= %1.14e\n", min);
    }
}

/* Minimização de funções convexas utilzando o método de newton
 */
double *newton(char *expressao, double *aproximacao_inicial, double epsilon, int max_iteracoes, int n_variaveis, double *value_funcao)
{
    int i;
    double tempo, tempo_derivadas, tempo_sistemas_lineares;
    tempo = 0;
    tempo_derivadas = 0.0;
    tempo_sistemas_lineares = 0.0;

    // aloca matriz gradiente[n_variaveis][1]  ==> matriz coluna (matriz que possui apenas uma coluna e n linhas)
    void **gradiente;
    gradiente = malloc(n_variaveis * sizeof(void *));
    for (i = 0; i < n_variaveis; i++)
    {
        gradiente[i] = malloc(1 * sizeof(void *));
    }

    // aloca matriz gradiente calculada ==> matriz coluna
    double *gradiente_calc;
    gradiente_calc = malloc(n_variaveis * sizeof(double));

    // aloca matriz hessiana[n_variaveis][n_variaveis]  ==> matriz quadrada
    void ***hessiana;
    hessiana = malloc(n_variaveis * sizeof(void *));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana[i] = malloc(n_variaveis * sizeof(void *));
    }

    // aloca matriz hessiana calculada[n_variaveis][n_variaveis]  ==> calcula a matriz nos pontos de aproximação da iteração
    double **hessiana_calc;
    hessiana_calc = malloc(n_variaveis * sizeof(double));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = malloc(n_variaveis * sizeof(double));
    }

    // aloca delta => matriz coluna
    double *delta;
    delta = malloc(n_variaveis * sizeof(double));

    //
    double *norma_gradiente_calc;
    norma_gradiente_calc = malloc(max_iteracoes * sizeof(double));

    gera_vetor_gradiente(gradiente, expressao, &tempo_derivadas);
    gera_matriz_hessiana(hessiana, gradiente, n_variaveis, &tempo_derivadas);

    printf("#Iteração \t| Newton Padrão \t| \n");

    for (i = 0; i < max_iteracoes; i++)
    {
        printf(" %d\t\t| ", i);
        value_funcao[i] = funcao_apresentacao(expressao, aproximacao_inicial, n_variaveis);
        if (value_funcao[i] == 1.00000000000000)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i-1]);
            printf("TEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }

        norma_gradiente_calc[i] = norma_grad(gradiente, gradiente_calc, aproximacao_inicial, n_variaveis);
        printf("%1.14e\t|\t\t\n", value_funcao[i]);

        if (norma_gradiente_calc[i] < epsilon)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
            printf("TEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }

        calcula_delta(hessiana, hessiana_calc, gradiente, gradiente_calc, delta, aproximacao_inicial, n_variaveis, &tempo_sistemas_lineares);
        proxima_aproximacao(delta, aproximacao_inicial, n_variaveis);

        if (norma_delta(delta, n_variaveis) < epsilon)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
            printf("TEMPO TOTAL: %1.14e\n", tempo);
            printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
            return aproximacao_inicial;
        }
    };

    tempo = timestamp();
    printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
    printf("TEMPO TOTAL: %1.14e\n", tempo);
    printf("TEMPO DERIVADAS: %1.14e\n", tempo_derivadas);
    printf("TEMPO SISTEMAS LINEARES: %1.14e\n", tempo_sistemas_lineares);
    return aproximacao_inicial;

    // libera a memória da hessiana
    for (i = 0; i < 1; i++)
        free(hessiana[i]);
    free(hessiana);

    // libera a memória da hessiana calc
    for (i = 0; i < 1; i++)
        free(hessiana_calc[i]);
    free(hessiana_calc);

    // libera a memória do gradiente
    for (i = 0; i < 1; i++)
        free(gradiente[i]);
    free(gradiente);
}
