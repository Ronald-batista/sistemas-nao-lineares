#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <string.h>
#include <assert.h>


void imprime_matriz(void **matriz_jacobiana, int linhas, int colunas)
{
    int i, j;

    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            printf("%s | ", evaluator_get_string(matriz_jacobiana[i]));
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

    // troca \n por \0 se for o caso
    //  length = strlen(expressao);
    //  if (length > 0 && expressao[length - 1] == "\n")
    //      expressao[length - 1] = "\0";

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
    for (i = 0; i < 1; i++){
        for (j = 0; j < n_variaveis; j++)
        {
            funcao_derivada_prim = evaluator_derivative(funcao, nomes_variaveis[j]);
            matriz_jacobiana[i]= funcao_derivada_prim;
            printf (" f’(x) = %s\n",evaluator_get_string(funcao_derivada_prim)); 
            printf (" f’(x) = %s\n",evaluator_get_string(matriz_jacobiana[j])); 
        }
    }

    imprime_matriz(matriz_jacobiana, 1, n_variaveis);
}


double norma(char *expressao, double *aproximacao_inicial){
    void *funcao;
    int contador;  /* Numero de variaveis na função*/
    char **nomes_variaveis;   

    // Criar expressão valida para a libmatheval
    funcao = evaluator_create(expressao);

    // preenche o vetor de nomes das variaveis e contador de variaveis
    evaluator_get_variables(funcao, &nomes_variaveis, &contador);

    double norma = evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial);
    if (norma < 0){ // mmodulo do resultado se necessario
        norma = norma * (-1);
    }
    printf("f(x) = %s\n", evaluator_get_string(funcao));
    printf("Contador = %d\n", contador);
    printf("Nomes Variaveis = %s\n", nomes_variaveis[0]);
    printf("Aproximação inicial = %lf\n", aproximacao_inicial[0]);
    printf("VALOR DA FUNÇÃO: %lf \n",evaluator_evaluate(funcao, contador, nomes_variaveis, aproximacao_inicial));
    //criar função
    //calcular funcao para a aproximacao inicial
    return norma;
}


