#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include <likwid.h>



void testaLikwid(){

    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("TESTE");
    double *X;
    X = (double *)malloc(10 * 10 * sizeof(double *));
    X[0] = 1.0;
    printf("%f\n", X[0]);

    LIKWID_MARKER_STOP("TESTE");
    LIKWID_MARKER_CLOSE;
   
}




void testaLeitura(){
    int i = 0;
    double *aproximacao_inicial;
    int n_variaveis, max_iteracoes, j;
    double epsilon;
    int contador = 0;
    int marcador = 1;
    int iterador = 0;

    char str[100];
    char buf[1000];
    while (scanf("%s", buf) != EOF)
    {
        if (marcador == 1)
        { // numero de variveis
            n_variaveis = atoi(buf);
            printf("n_variaveis: %d\n", n_variaveis);
            aproximacao_inicial = malloc(n_variaveis * sizeof(double *));
            marcador++;
            contador = n_variaveis;
        }
        else if (marcador == 2)
        { // expressao
            strcpy(str, buf);
            marcador++;
            printf("expressao: %s\n", str);
        }
        else if ((marcador == 3) && (contador > 0))
        { // aproximacao inicial
            aproximacao_inicial[iterador] = atof(buf);
            contador--;
            iterador++;
            if (contador == 0)
            {
                marcador++;
                printf("aproximacao inicial: [");
                for (i = 0; i < n_variaveis; i++)
                { // imprime as aproximacoes iniciais
                    printf("  %f  ", aproximacao_inicial[i]);
                }
                printf("]\n");
            }
        }
        else if (marcador == 4)
        { // epsilon
            epsilon = atof(buf);
            marcador++;
            printf("epsilon: %1.14e\n", epsilon);
        }
        else if (marcador == 5)
        { // maximo de iteracoes
            max_iteracoes = atoi(buf);
            marcador = 1;
            printf("max_iteracoes: %d\n", max_iteracoes);
            contador = 0;
            iterador = 0;
            printf("\n");
            free(aproximacao_inicial);
        }
        else
        {
            printf("ERRO\n");
            break;
        }
    }

}





void testaAlocacaoContigua(){
      double *matriz;
    int n = 10;
    matriz = (double *)malloc(n * n * sizeof(double));

    //zerando a matriz
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            matriz[i * n + j] = 0;
        }
    }

    //preenchendo a matriz
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            matriz[i * n + j] = i * n + j;
        }
    }
    matriz[0*n] = 0.0;
    matriz[1*n] = 0.0;
    matriz[2*n] = 0.0;
    //imprimindo a matriz
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%f ", matriz[i * n + j]);
        }
        printf("\n");
    }


    free(matriz);
}

