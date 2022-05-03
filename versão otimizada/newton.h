#include "newton.c"

/* calcula a matriz hessiana nos pontos de aproximação da iteração */
void calcula_matriz_hessiana(double **matriz_hessiana_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas);


/* calcula o vetor gradiente nos pontos de aproximação da iteração */
void calcula_vetor_gradiente(double *gradiente_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas);


/*  Encontra o elemento de maior valor de uma matriz na linha i e retorna a sua posição */
uint encontraMax(double **matriz, int linha, int coluna);


/*  troca a linha i da matriz, pela linha ipivo. Ocorre a mesma troca no vetor dos elementos independentes. */
void trocaLinha(double **matriz, double *vetor_independente, int i, uint ipivo, int max_col);


/* Seja um S.L. de ordem 'n' realiza a eliminação de gauss na matriz com os termos independentes */
void eliminacaoGauss(double **matriz, double *vetor_independente, uint n);


/* Seja um S.L. triangula de orden n, encontra as suas raizes e armazena no vetor delta */
void retrossubs(double **matriz, double *vetor_independente, double *delta, uint n);


/* Calcula a norma do vetor gradientes nos pontos x^i */
double norma_grad(double *gradiente_calc, double *aproximacao_inicial, int n_variaveis, double *tempo_derivadas);


/* Calcula a norma de f(x) nos pontos x^i */
double funcao_apresentacao(void *expressao, double *aproximacao_inicial, int n_variaveis);


/* Calcula o delta ==> resolve o sistema linear H(X^(i))𝚫^(i) = -𝛻f (X^(i)) */
void calcula_delta(double **hessiana_calc, double *gradiente_calc, double *delta, double *aproximacao_inicial, int n_variaveis, double *tempo_sistemas_lineares, double *tempo_derivadas);


/* Gera os proximos pontos de aproximação */
void proxima_aproximacao(double *delta, double *aproximacao_inicial, int n_variaveis);


/*  Encontra a norma do vetor delta */
double norma_delta(double *delta, int n_variaveis);

/* Imprime o valor minimo de um vetor double */
// void minimo_global(double *norma_funcao, int max_iteracoes);


/* Minimização de funções convexas utilzando o método de newton */
double *newton(char *expressao, double *aproximacao_inicial, double epsilon, int max_iteracoes, int n_variaveis, double *value_funcao);
