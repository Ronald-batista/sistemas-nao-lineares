#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.c"
#include "utils/Rosenbrock.c"
#include <likwid.h>


/* calcula a matriz hessiana nos pontos de aproximação da iteração */
void calcula_matriz_hessiana(double *matriz_hessiana_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas)
{
    int i, j;
    int DIMENSAO = n_variaveis;
    double tempo = timestamp();
    string_t markername;
    markername = markerName("Matriz_Hessiana:", DIMENSAO);
    LIKWID_MARKER_START(markername);
    // calcula a matriz hessiana = otimizado com a tecnica de unrolling
    for (i = 0; i < (n_variaveis - (n_variaveis % 4)); i += 4)
    {
        for (j = 0; j < (n_variaveis - (n_variaveis % 4)); j += 4)
        {
            matriz_hessiana_calc[i * n_variaveis + j] = rosenbrock_dxdy(i, j, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 1] = rosenbrock_dxdy(i, j + 1, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 2] = rosenbrock_dxdy(i, j + 2, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 3] = rosenbrock_dxdy(i, j + 3, aproximacao_inicial, DIMENSAO);
        }
    }
    // residuo
    for (i = (n_variaveis - (n_variaveis % 4)); i < n_variaveis; i++)
    {
        for (j = (n_variaveis - (n_variaveis % 4)); j < n_variaveis; j++)
        {
            matriz_hessiana_calc[i * n_variaveis + j] = rosenbrock_dxdy(i, j, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 1] = rosenbrock_dxdy(i, j + 1, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 2] = rosenbrock_dxdy(i, j + 2, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j + 3] = rosenbrock_dxdy(i, j + 3, aproximacao_inicial, DIMENSAO);
        }
    }
    LIKWID_MARKER_STOP(markername);

    tempo = timestamp() - tempo; // tempo gasto para o calculo das derivadas
    *tempo_derivadas = *tempo_derivadas + tempo;
}

/* calcula o vetor gradiente nos pontos de aproximação da iteração
 */
void calcula_vetor_gradiente(double *gradiente_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas)
{
    int DIMENSAO = n_variaveis;
    int i;
    double tempo = timestamp();

    string_t markername;
    markername = markerName("Vetor_Gradiente:", DIMENSAO);
    LIKWID_MARKER_START(markername);
    for (i = 0; i < n_variaveis; i++)
        gradiente_calc[i] = rosenbrock_dx(i, aproximacao_inicial, DIMENSAO);
    LIKWID_MARKER_STOP(markername);

    tempo = timestamp() - tempo; // tempo gasto para o calculo das derivadas
    *tempo_derivadas = *tempo_derivadas + tempo;
}

/*  Encontra o elemento de maior valor de uma matriz na linha i e retorna a sua posição
 */
uint encontraMax(double *matriz, int linha, int coluna)
{
    double max = matriz[0];
    uint indice_pivo = 0;
    double modulo;
    int i, j;
    for (i = 0; i < linha; i++)
    {
        for (j = coluna; j < coluna + 1; j++)
        {
            if (matriz[i * coluna + j] < 0)
                modulo = matriz[i * coluna + j] * (-1);
            else
                modulo = matriz[i * coluna + j];

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
void trocaLinha(double *matriz, double *vetor_independente, int i, uint ipivo, int max_col)
{
    // double **matriz_temporaria;
    int linha, coluna;
    double temp;

    // troca linhas matriz
    for (linha = i; linha < i + 1; linha++)
    {
        for (coluna = 0; coluna < max_col; coluna++)
        {
            temp = matriz[linha * max_col + coluna];                             // matriz[linha][coluna]
            matriz[linha * max_col + coluna] = matriz[ipivo * max_col + coluna]; // matriz[ipivo*max_col+coluna]
            matriz[ipivo * max_col + coluna] = temp;
        }
    }

    // troca do vetor independente
    temp = vetor_independente[i];
    vetor_independente[i] = vetor_independente[ipivo];
    vetor_independente[ipivo] = temp;
}

/* Seja um S.L. de ordem 'n' realiza a eliminação de gauss na matriz com os termos independentes
 */
void eliminacaoGauss(double *matriz, double *vetor_independente, uint n)
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
            if (matriz[i * n + i] == 0)
                goto next;
            double m = matriz[k * n] / matriz[i * n + i]; // m =  matriz[k][i] / matriz[i][i]
            matriz[k * n] = 0.0;
            for (int j = i + 1; j < n; ++j)
            {
                matriz[k * n + j] -= matriz[i * n + j] * m; // matriz[k][j] -= matriz[i][j] * m;
            }

            vetor_independente[k] -= vetor_independente[i] * m;
        next:;
        }
    }
}

/* Seja um S.L. triangula de orden n, encontra as suas raizes e armazena no vetor delta

 */
void retrossubs(double *matriz, double *vetor_independente, double *delta, uint n)
{
    int i, j;
    
    if (n == 1)
    {
        delta[0] = vetor_independente[0] / matriz[0];
        return;
    }

    for (int i = n - 1; i >= 0; --i)
    {
        delta[i] = vetor_independente[i];
        for (int j = i + 1; j < n; ++j)
            delta[i] -= matriz[i * n + j] * delta[j];
        delta[i] /= matriz[i * n + i];
    }
}

/* Calcula a norma do vetor gradiente nos pontos x^i
 */
double norma_grad(double *gradiente_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas)
{
    int i;
    double norma, temp;
    calcula_vetor_gradiente(gradiente_calc, aproximacao_inicial, n_variaveis, tempo_derivadas);

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
    int DIMENSAO = n_variaveis;
    double valor;
    valor = rosenbrock(aproximacao_inicial, DIMENSAO);
    if (valor > 0 || valor < 0) // verifica se é um numero válido
        return valor;
    else
        return 1.00000000000000; // condicao em que a funcao de rosenbrock não pôde ser calculada
}

/* Calcula o delta ==> resolve o sistema linear H(X^(i))𝚫^(i) = -𝛻f (X^(i))
 */
void calcula_delta(double *hessiana_calc, double *gradiente_calc, double *delta, double *aproximacao_inicial, int n_variaveis, double *tempo_sistemas_lineares, double *tempo_derivadas)
{
    int DIMENSAO = n_variaveis;
    int i;
    double tempo;
    calcula_matriz_hessiana(hessiana_calc, aproximacao_inicial, n_variaveis, tempo_derivadas);

    // multiplica o gradiente calculado por -1
    for (i = 0; i < (n_variaveis - (n_variaveis % 4)); i += 4)
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
        gradiente_calc[i + 1] = gradiente_calc[i + 1] * (-1);
        gradiente_calc[i + 2] = gradiente_calc[i + 2] * (-1);
        gradiente_calc[i + 3] = gradiente_calc[i + 3] * (-1);
    }
    // Residuo
    for (i = (n_variaveis - (n_variaveis % 4)); i < n_variaveis; i++)
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
    }

    tempo = timestamp();
    string_t markername;
    markername = markerName("Resolucao_Sistema_Linear:", DIMENSAO);
    LIKWID_MARKER_START(markername);
    eliminacaoGauss(hessiana_calc, gradiente_calc, n_variaveis);
    retrossubs(hessiana_calc, gradiente_calc, delta, n_variaveis);
    LIKWID_MARKER_STOP(markername);

    tempo = timestamp() - tempo; // tempo para resolução do sistema linear (eliminação de gauss + retrosubstituição)
    *tempo_sistemas_lineares = *tempo_sistemas_lineares + tempo;
}

/* Gera os proximos pontos de aproximação
 */
void proxima_aproximacao(double *delta, double *aproximacao_inicial, int n_variaveis)
{
    int i;
    for (i = 0; i < (n_variaveis - (n_variaveis % 4)); i += 4)
    {
        aproximacao_inicial[i] = aproximacao_inicial[i] + delta[i];
        aproximacao_inicial[i + 1] = aproximacao_inicial[i + 1] + delta[i + 1];
        aproximacao_inicial[i + 2] = aproximacao_inicial[i + 2] + delta[i + 2];
        aproximacao_inicial[i + 3] = aproximacao_inicial[i + 3] + delta[i + 3];
    }
    // residuo
    for (i = (n_variaveis - (n_variaveis % 4)); i < n_variaveis; i++)
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

/* Minimização de funções convexas utilzando o método de newton
 */
double *newton(char *expressao, double *aproximacao_inicial, double epsilon, int max_iteracoes, int n_variaveis, double *value_funcao)
{   
    int DIMENSAO = n_variaveis;
    int i;
    double tempo, tempo_derivadas, tempo_sistemas_lineares;
    tempo = 0;
    tempo_derivadas = 0.0;
    tempo_sistemas_lineares = 0.0;

    // aloca matriz gradiente calculada ==> matriz coluna
    double *gradiente_calc;
    gradiente_calc = (double *)malloc(n_variaveis * sizeof(double));

    // aloca matriz hessiana calculada NxN ==> armazena o calculo da matriz nos pontos de aproximação da iteração
    double *hessiana_calc;
    hessiana_calc = (double *)malloc(n_variaveis * n_variaveis * sizeof(double *));
    // memset(hessiana_calc, 0, (n_variaveis*n_variaveis) * (sizeof(double) ));

    // aloca delta => matriz coluna
    double *delta;
    delta = (double *)malloc(n_variaveis * sizeof(double));

    //
    double *norma_gradiente_calc;
    norma_gradiente_calc = (double *)malloc(max_iteracoes * sizeof(double));

    // gera_vetor_gradiente(gradiente, expressao, &tempo_derivadas);
    // gera_matriz_hessiana(hessiana, gradiente, n_variaveis, &tempo_derivadas);

    printf("#Iteração \t| Newton Padrão \t| \n");

    for (i = 0; i < max_iteracoes; i++)
    {
        printf(" %d\t\t| ", i);
        value_funcao[i] = funcao_apresentacao(expressao, aproximacao_inicial, n_variaveis);
        if (value_funcao[i] == 1.00000000000000)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i - 1]);
            printf("TEMPO TOTAL_%d: %1.14e\n", DIMENSAO, tempo);
            printf("TEMPO DERIVADAS_%d: %1.14e\n", DIMENSAO, tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES_%d: %1.14e\n", DIMENSAO, tempo_sistemas_lineares);
            // LIKWID_MARKER_STOP(markername);
            return aproximacao_inicial;
        }

        norma_gradiente_calc[i] = norma_grad(gradiente_calc, aproximacao_inicial, n_variaveis, &tempo_derivadas);
        printf("%1.14e\t|\t\t\n", value_funcao[i]);

        if (norma_gradiente_calc[i] < epsilon)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
            printf("TEMPO TOTAL_%d: %1.14e\n", DIMENSAO, tempo);
            printf("TEMPO DERIVADAS_%d: %1.14e\n", DIMENSAO, tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES_%d: %1.14e\n", DIMENSAO, tempo_sistemas_lineares);
            // LIKWID_MARKER_STOP(markername);
            return aproximacao_inicial;
        }

        calcula_delta(hessiana_calc, gradiente_calc, delta, aproximacao_inicial, n_variaveis, &tempo_sistemas_lineares, &tempo_derivadas);
        proxima_aproximacao(delta, aproximacao_inicial, n_variaveis);

        if (norma_delta(delta, n_variaveis) < epsilon)
        {
            tempo = timestamp();
            printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
            printf("TEMPO TOTAL_%d: %1.14e\n", DIMENSAO, tempo);
            printf("TEMPO DERIVADAS_%d: %1.14e\n", DIMENSAO, tempo_derivadas);
            printf("TEMPO SISTEMAS LINEARES_%d: %1.14e\n", DIMENSAO, tempo_sistemas_lineares);
            // LIKWID_MARKER_STOP(markername);
            return aproximacao_inicial;
        }
    };

    tempo = timestamp();
    printf("\nMinimo Global: %1.14e\n", value_funcao[i]);
    printf("TEMPO TOTAL_%d: %1.14e\n", DIMENSAO, tempo);
    printf("TEMPO DERIVADAS_%d: %1.14e\n", DIMENSAO,  tempo_derivadas);
    printf("TEMPO SISTEMAS LINEARES_%d: %1.14e\n", DIMENSAO, tempo_sistemas_lineares);
    // LIKWID_MARKER_STOP(markername);

    // libera a memória da hessiana calc
    free(hessiana_calc);

    // libera a memória do gradiente calc
    free(gradiente_calc);

    // libera a memória do delta
    free(delta);

    // libera a memória do vetor de norma gradiente
    free(norma_gradiente_calc);

    
    return aproximacao_inicial;
}
