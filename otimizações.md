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


