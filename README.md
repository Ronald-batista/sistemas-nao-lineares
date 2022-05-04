# Otimizações realizadas

### Alocação da matriz hessiana
#### Era:
    double **hessiana_calc;
    hessiana_calc = malloc(n_variaveis * sizeof(double));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = malloc(n_variaveis * sizeof(double));
    }
#### Foi alterado para:
    double *hessiana_calc; 
    hessiana_calc = (double *)malloc(n * n * sizeof(double));
#### Justificativa:
Alocar a memória de forma contigua. Lembrando que o acesso aos elementos da matriz agora se dá por Matriz[i * n + j].
#### Consequencias:
Todas as funções que recebem essa matriz como parametro agora é necessario alterar a maneira que os elementos são percorridos


### Alocação do vetor gradiente
#### Era:
    double *gradiente_calc;
    gradiente_calc = malloc(n_variaveis * sizeof(double));
#### Foi alterado para: 
    double *gradiente_calc;
    gradiente_calc = (double *) malloc(n_variaveis * sizeof(double));
#### Justificativa:
Correção da forma alocação e alocação da memória de forma contigua. 
    

### Alocação do delta
#### Era:
    double *delta;
    delta = malloc(n_variaveis * sizeof(double));
#### Foi alterado para: 
    double *delta;
    delta = (double*) malloc(n_variaveis * sizeof(double));
#### Justificativa:
Correção da forma alocação e alocação da memória de forma contigua. 


### Alocação do vetor da norma do gradiente
#### Era:
    double *norma_gradiente_calc;
    norma_gradiente_calc = malloc(max_iteracoes * sizeof(double));
#### Foi alterado para: 
    double *norma_gradiente_calc;
    norma_gradiente_calc = (double*) malloc(max_iteracoes * sizeof(double));
#### Justificativa:
Correção da forma alocação e alocação da memória de forma contigua. 


### Unroll na função "calcula_delta"
#### Era:
    // multiplica o gradiente calculado por -1
    for (i = 0; i < (n_variaveis; i++)
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
    }
#### Foi alterado para:
       // multiplica o gradiente calculado por -1
    for (i = 0; i < (n_variaveis-(n_variaveis%4)); i+=4)
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
        gradiente_calc[i+1] = gradiente_calc[i+1] * (-1);
        gradiente_calc[i+2] = gradiente_calc[i+2] * (-1);
        gradiente_calc[i+3] = gradiente_calc[i+3] * (-1);
    }
    //Residuo 
    for(i = (n_variaveis-(n_variaveis%4)); i < n_variaveis; i++) 
    {
        gradiente_calc[i] = gradiente_calc[i] * (-1);
    }
#### Justificativa
tem o objetivo de incrementar o speed-up da aplicação através da diminuição das instruções que controlam o laço, diminuindo o número de iterações.


### Unroll na função "calcula_matriz_hessiana"
#### Era:
        // calcula a matriz hessiana = otimizado com a tecnica de unrolling
    for (i = 0; i < n_variaveis; i++)
    {
        for (j = 0; j <n_variaveis ; j++)
        {
            matriz_hessiana_calc[i * n_variaveis + j] = rosenbrock_dxdy(i, j, aproximacao_inicial, DIMENSAO);
        }
    }
    
#### Foi alterado para:
    // calcula a matriz hessiana = otimizado com a tecnica de unrolling
    for (i = 0; i < (n_variaveis-(n_variaveis%4)); i+=4)
    {
        for (j = 0; j <(n_variaveis-(n_variaveis%4)); j+=4)
        {
            matriz_hessiana_calc[i * n_variaveis + j] = rosenbrock_dxdy(i, j, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+1] = rosenbrock_dxdy(i, j+1, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+2] = rosenbrock_dxdy(i, j+2, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+3] = rosenbrock_dxdy(i, j+3, aproximacao_inicial, DIMENSAO);
        }
    }
    // residuo
       for (i = (n_variaveis-(n_variaveis%4)); i < n_variaveis; i++)
    {
        for (j = (n_variaveis-(n_variaveis%4)); j < n_variaveis; j++)
        {
            matriz_hessiana_calc[i * n_variaveis + j] = rosenbrock_dxdy(i, j, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+1] = rosenbrock_dxdy(i, j+1, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+2] = rosenbrock_dxdy(i, j+2, aproximacao_inicial, DIMENSAO);
            matriz_hessiana_calc[i * n_variaveis + j+3] = rosenbrock_dxdy(i, j+3, aproximacao_inicial, DIMENSAO);
        }
    }
#### Justificativa
tem o objetivo de incrementar o speed-up da aplicação através da diminuição das instruções que controlam o laço, diminuindo o número de iterações.



### Unroll na função "proxima_approximacao"
#### Era:
   for (i = 0; i < n_variaveis; i++)
    {
        aproximacao_inicial[i] = aproximacao_inicial[i] + delta[i];
    }
#### Foi alterado para:
    
#### Justificativa
tem o objetivo de incrementar o speed-up da aplicação através da diminuição das instruções que controlam o laço, diminuindo o número de iterações.
