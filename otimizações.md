# Otimizações realizadas

### Alocação do vetor gradiente
#### Era:
    double *gradiente_calc;
    gradiente_calc = malloc(n_variaveis * sizeof(double));
#### Foi alterado para: 
    double *gradiente_calc;
    gradiente_calc = (double *) malloc(n_variaveis * sizeof(double));
#### Justificativa:
Alocar a memória de forma contigua.
    

### Alocação da matriz hessiana
#### Era:
    double **hessiana_calc;
    hessiana_calc = malloc(n_variaveis * sizeof(double));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = malloc(n_variaveis * sizeof(double));
    }
#### Foi alterado para: 
    double **hessiana_calc;
    hessiana_calc = (double **) malloc(n_variaveis * sizeof(double*));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = (double *) malloc(n_variaveis * sizeof(double));
    }
#### Justificativa:
Alocar a memória de forma contigua.
    

### Alocação do delta
#### Era:
    double *delta;
    delta = malloc(n_variaveis * sizeof(double));
#### Foi alterado para: 
    double *delta;
    delta = (double*) malloc(n_variaveis * sizeof(double));
#### Justificativa:
Alocar a memória de forma contigua.


### Alocação do vetor da norma do gradiente
#### Era:
    double *norma_gradiente_calc;
    norma_gradiente_calc = malloc(max_iteracoes * sizeof(double));
#### Foi alterado para: 
    double *norma_gradiente_calc;
    norma_gradiente_calc = (double*) malloc(max_iteracoes * sizeof(double));
#### Justificativa:
Alocar a memória de forma contigua.



gcc teste.c -DLIKWID_PERFMON -O3 -mavx -march=native -o teste  -L${LIKWID_LIB} -llikwid