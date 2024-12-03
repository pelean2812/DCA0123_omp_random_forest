#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

#include "stdafx.h"

#define MAX_SIZE 1000

typedef struct matriz{
    double M[MAX_SIZE][MAX_SIZE];
    int i;
    int j;
}matriz;

typedef struct train_test{
    matriz* train;
    matriz* test;
} TrainTest;

matriz* aloca_struct_matriz();

void libera_matriz(matriz* m);
void imprime_matriz(const matriz* m);

matriz* insere_matriz(matriz* m, char* arq);

matriz* coleta_dados(char* arq);
matriz* amostra_aleatoria(int n, matriz* m, matriz* m_a);
int* rand_sem_rep(matriz* m, int n, int* numeros);
TrainTest train_test_split(matriz* dataset, double p);

#endif // MATRIZ_H_INCLUDED
