#ifndef VETOR_H_INCLUDED
#define VETOR_H_INCLUDED

#include "stdafx.h"
#include "matriz.h"

typedef struct vetor_double {
    double* v;
    size_t n;
} Vetor;

typedef struct grupo_vetor {
    Vetor* v;
    size_t n;
} Grupo;

typedef struct grupos_grupo {
    Grupo* v;
    size_t n;
} Grupos;

void imprimir_vetor(Vetor v);
void imprimir_grupos(Grupos g);
unsigned int contar(Vetor vet, double a);
void quicksort(double values[], int began, int end);
void eleminar_repetido(Vetor* vet);
matriz* grupo_to_matriz(Grupo g);
Grupo* matriz_to_grupo(matriz* m);
double contar_maior(Vetor vet);
void desaloca_vetor(Vetor vet);
void desaloca_grupo(Grupo g);
void desaloca_grupos(Grupos g);

#endif /* VETOR_H */

