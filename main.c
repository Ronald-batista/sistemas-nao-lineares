#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "mylib.h"

int main (){
    int n_variaveis, max_iteracoes;
    double  aproximacao_inicial, epsilon;
    char expressao[50];
    printf("Digite o numero de variaveis que função possui: \n");
    scanf("%d", &n_variaveis);

    printf("Digite a Expressão da função (não utilize espaços): \n");
    scanf("%s", expressao);

    printf("Digite a aproximação inicial: \n");
    scanf("%lf", &aproximacao_inicial);

    printf("Digite a tolerancia (EPSILON) para o criterio de parada: \n");
    scanf("%lf", &epsilon);

    printf("Digitea quantidade maxima de iterações permitidas: \n");
    scanf("%d", &max_iteracoes);

    printf("%d\n", n_variaveis);
    printf("%s\n", expressao);  
    printf("%lf\n", aproximacao_inicial);
    printf("%lf\n", epsilon);
    printf("%d\n", max_iteracoes);


    
    //double resultado = newton();

    return 0;
}


double *newton (char const *expressao, double *aproximacao_inicial, double const epsilon, int const max_iteracoes, int const n_variaveis){
    int i,linhas;
    double *delta = malloc (n_variaveis *sizeof(double));

    // calcular matriz jacobiana
    void **matriz_jacobiana;
    matriz_jacobiana =  malloc (linhas * sizeof(void*));
    for(i=0; i < n_variaveis; i++){
        matriz_jacobiana[i] = malloc(n_variaveis * sizeof(void));
    }
    
    calcular_matriz_jacobiana(matriz_jacobiana, expressao, n_variaveis);

    for (i=0; i < max_iteracoes;i++){
        // Verifica criterio de parada - Norma da função na iteração atual é menor que o epsilon?
        if (norma(expressao, aproximacao_inicial) < epsilon)
            return aproximacao_inicial;
        
        //resolve o sistema linear para encontrar o delta. J(X^i)*DELTA^i = -F(X^i)
        eliminacaoGauss(matriz_jacobiana, delta, calcula_expressao(expressao, aproximacao_inicial, i));

        //proxima aproximação
        proxima_aproximacao(aproximacao_inicial, delta);
        
        if (norma(delta, aproximacao_inicial) < epsilon){
            return aproximacao_inicial;
        }
    }
}


