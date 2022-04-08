#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>
#include "utils/utils.h"
#include "novo.h"
#include <sys/stat.h>

int main()
{
    int n_variaveis, max_iteracoes, i, j;
    double epsilon = 0.0000001;
    char string[] = "7*x1-log(x1)";
    char *expressao = string;
    double *resultado;
    double *aproximacao_inicial;
    max_iteracoes = 20;
    n_variaveis = 1;
    aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
    aproximacao_inicial[0] = 0.1;
    // aproximacao_inicial[1] = 0.05;

    // aloca matriz gradiente[n_variaveis][1]  ==> matriz coluna (matriz que possui apenas uma coluna e n linhas)
    // void **gradiente;
    // gradiente = malloc(n_variaveis * sizeof(void *));
    // for (i = 0; i < n_variaveis; i++)
    // {
    //     gradiente[i] = malloc(1 * sizeof(void *));
    // }

    // // aloca matriz gradiente calculada ==> matriz coluna
    // double *gradiente_calc;
    // gradiente_calc = malloc(n_variaveis * sizeof(double));

    // // aloca matriz hessiana[n_variaveis][n_variaveis]  ==> matriz quadrada
    // void ***hessiana;
    // hessiana = malloc(n_variaveis * sizeof(void *));
    // for (i = 0; i < n_variaveis; i++)
    // {
    //     hessiana[i] = malloc(n_variaveis * sizeof(void *));
    // }

    // // aloca matriz hessiana calculada[n_variaveis][n_variaveis]  ==> calcula a matriz nos pontos de aproximação da iteração
    // double **hessiana_calc;
    // hessiana_calc = malloc(n_variaveis * sizeof(double));
    // for (i = 0; i < n_variaveis; i++)
    // {
    //     hessiana_calc[i] = malloc(n_variaveis * sizeof(double));
    // }

    // // aloca delta => matriz coluna
    // double *delta;
    // delta = malloc(n_variaveis * sizeof(double));

    //  gera_vetor_gradiente(gradiente, expressao);
    //  gera_matriz_hessiana(hessiana, gradiente, n_variaveis);
    //  calcula_vetor_gradiente(gradiente,gradiente_calc, aproximacao_inicial, n_variaveis);
    //     calcula_matriz_hessiana(hessiana, hessiana_calc, aproximacao_inicial, n_variaveis);
    //     norma_gradiente(gradiente, gradiente_calc, aproximacao_inicial,n_variaveis);

    // calcula_delta(hessiana, hessiana_calc, gradiente, gradiente_calc, delta, aproximacao_inicial, n_variaveis);
    // for(i=0; i< n_variaveis; i++)
    //     printf("delta [%d] = %lf", i, delta[i]);
    // printf("\n");
    // proxima_aproximacao(delta,aproximacao_inicial,n_variaveis);
    // uint ipivo = encontraMax(hessiana_calc, n_variaveis, 0);
    // trocaLinha(hessiana_calc, gradiente_calc,0,ipivo, n_variaveis );
    // eliminacaoGauss(hessiana_calc,gradiente_calc, n_variaveis );
    // retrossubs(hessiana_calc,gradiente_calc,delta,n_variaveis);
    // --------------------------------------------------------------------------------------------------------------------
    //     const char *filename = "funcoes.dat";
    //     char c = '\n';
    //     FILE *in_file = fopen(filename, "r");
    //     if (!in_file)
    //     {
    //         perror("fopen");
    //         exit(EXIT_FAILURE);
    //     }

    //     struct stat sb;
    //     if (stat(filename, &sb) == -1)
    //     {
    //         perror("stat");
    //         exit(EXIT_FAILURE);
    //     }

    //     char *file_contents = malloc(sb.st_size);

    //    int caractere, existe_linhas = 0;
    //   int quant_linhas = 0;
    //     while ((caractere = fgetc(in_file)) != EOF)
    //     {
    //         existe_linhas = 1; // há conteúdo no arquivo

    //         if (caractere == '\n')
    //         { // é uma quebra de linha?
    //             // vamos incrementar a quantidade de linhas
    //             quant_linhas++;
    //         }
    //     }
    //     printf("QTD LINHDA: %d\n", quant_linhas);
    //         printf("-------------------------------------------------------\n");
    //         fscanf(in_file, "%d %s %lf %lf %d", &n_variaveis, expressao, aproximacao_inicial, &epsilon, &max_iteracoes);
    //         printf("EPSILON: %1.14e\n", epsilon);
    //         printf("APROXIMACAO INICIAL: %lf\n", aproximacao_inicial[0]);
    //         printf("numero varaiveis: %d\n", n_variaveis);
    //         printf("Max Iterações: %d\n", max_iteracoes);
    //         void *f;
    //         f = evaluator_create(expressao);
    //         printf("F(x)= %s\n", evaluator_get_string(f));

    //         printf("-------------------------------------------------------\n");
    //         i++;

    //     fclose(in_file);
    //     exit(EXIT_SUCCESS);

    double *norma_funcao;

    norma_funcao = malloc(max_iteracoes * sizeof(double));
    double *newtin;
    newtin = newton(expressao, aproximacao_inicial, epsilon, max_iteracoes, n_variaveis, norma_funcao);
    minimo_global(norma_funcao, max_iteracoes);
}

