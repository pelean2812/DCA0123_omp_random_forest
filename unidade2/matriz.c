#include "matriz.h"

#define MAX_SIZE 1000

//alocaa matriz
matriz* aloca_struct_matriz()
{
    matriz* m = (matriz*)malloc(sizeof(matriz));
    m->i = 1;
    m->j = 1;
    return m;
}

//desaloca a matriz
void libera_matriz(matriz* m)
{
    free(m);
}

//imprime a matriz
void imprime_matriz(const matriz* m)
{
    for(int k = 0; k < m->i; k++)
    {
        for(int p = 0; p < m->j; p++)
        {
            printf("%lf ", m->M[k][p]);
        }
        printf ("\n");
    }
}

//insere os dados do arquivo na matriz
matriz* insere_matriz(matriz* m, char* arq)
{
    FILE *dados;
    int k = 0, p = 0;
    char *str, *token, linha[30];
    double numero;
    dados = fopen(arq, "rt");

    if(dados == NULL)
    {
        printf("Erro na leitura do arquivo.\n");
        exit(1);
    }

    while(!feof(dados))
    {
        p = 0;
        str = fgets(linha, 30, dados);
        token = strtok(str, ",");
        numero = atof(token);
        m->M[k][p] = numero;
        while(token != NULL)
        {
            token = strtok(NULL, ",");
            if(token != NULL) {
                numero = atof(token);
                p++;
                m->j = p;
                m->M[k][p] = numero;
            }

        }
        k++;
        m->i = k;
    }
    m->j += 1;
    fclose(dados);
    return m;
}

//cria um vetor de números aleatórios de indices sem reposição
int* rand_sem_rep(matriz* m, int n, int* numeros) {
    numeros = malloc(m->i * sizeof(int));

    for (int i = 0; i < m->i; i++) {
        numeros[i] = i;
    }

    for (int i = n; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = numeros[j];
        numeros[j] = numeros[i];
        numeros[i] = tmp;
    }

    return numeros;
}

//pega uma amostra aleatoria da matriz m e coloca na matriz m_a
matriz* amostra_aleatoria(int n, matriz* m, matriz* m_a) {
    if(n > m->i){
        printf("Amostra muito grande.\n");
        exit(1);
    }

    int *numeros = NULL;
    numeros = rand_sem_rep(m, n, numeros);
    for(int k = 0; k < n; k++){
        for(int p = 0; p < m->j; p++){
            m_a->M[k][p] = m->M[numeros[k]][p];
        }
    }

    m_a->j = m->j;
    m_a->i = n;
    free(numeros);
    return m_a;
}

//aloca a matriz e coleta dados do aquivo em csv
matriz* coleta_dados(char* arq)
{
    matriz* m = NULL;
    m = aloca_struct_matriz();
    m = insere_matriz(m, arq);
    return m;
}

//separa p porcento dos dados aleatoriamente para treinar a arvore e (1 - p) porcento para testar a arvore
TrainTest train_test_split(matriz* dataset, double p) {
    
    if(p > 1){
        printf("Proporcao maior que zero\n");
        exit(1);
    }
    matriz *train, *test;
    int n_train, n_test;
    n_train = (int)(p * dataset->i);
    n_test = dataset->i - n_train;

    train = (matriz*)malloc(sizeof(matriz));
    test = (matriz*)malloc(sizeof(matriz));

    train->i = n_train;
    test->i = n_test;
    train->j = dataset->j;
    test->j = dataset->j;

    int *numeros = NULL;
    numeros = rand_sem_rep(dataset, dataset->i - 1, numeros);

    for(int k = 0; k < n_train; k++){
        for(int p = 0; p < dataset->j; p++){
            train->M[k][p] = dataset->M[numeros[k]][p];
        }
    }

    for(int k = 0; k < n_test; k++){
        for(int p = 0; p < dataset->j; p++){
            test->M[k][p] = dataset->M[numeros[k]][p];
        }
    }

    free(numeros);

    TrainTest tt;
    tt.train = train;
    tt.test = test;
    return tt;
}
