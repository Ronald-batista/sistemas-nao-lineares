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
    double **hessiana_calc;
    hessiana_calc = (double **) malloc(n_variaveis * sizeof(double*));
    for (i = 0; i < n_variaveis; i++)
    {
        hessiana_calc[i] = (double *) malloc(n_variaveis * sizeof(double));
    }
#### Justificativa:
Alocar a memória de forma contigua.
    
