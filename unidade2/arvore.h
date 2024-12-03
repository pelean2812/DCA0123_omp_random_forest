#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#include "stdafx.h"
#include "Vetor.h"
#include "matriz.h"

typedef struct no No;

typedef struct no{
    size_t index;
    double value;
    Grupos grupos;
    No* left;
    No* right;
    double v_left, v_right;
    char folha;
} No;

No* criar_no();
double gini_index(Grupos grupos, Vetor classes);
Grupos test_split(unsigned int index, double value, matriz* dataset);
No* get_split(matriz* dataset);
double to_terminal(Grupo g);
void split(No* node, int max_depth, int min_size, int depth);
No* build_tree(matriz* m, int max_depth, int min_size);
void imprimir_arvore(No* node, int depth);
double predict(No* node, Vetor row);
void desaloca_arvore(No* arv);

#endif
