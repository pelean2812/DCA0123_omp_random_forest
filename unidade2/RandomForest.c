#include "RandomForest.h"

//Cria Random forest alocando várias decision trees
RandomForest* cria_random_forest(int tam, int amostra_min, int altura_max, matriz* dataset) {
    RandomForest* arv=NULL;
    matriz* ale = aloca_struct_matriz();
    //srand(time(NULL));
    int amostra;

    int i, altura;
    altura = altura_max;
    
    if(amostra_min >= dataset->i) {
        amostra_min = 1;
    }

    arv = (RandomForest*)malloc(sizeof(RandomForest));
    arv->n = tam;
    arv->v_arv = (No**)malloc(tam *sizeof(No*));
    
    for(i=0; i < tam; i++) {
        amostra = amostra_min + ((rand()) % (dataset->i - 1 - amostra_min));
        altura = 1 + (rand() % altura_max);

        //printf("Arvore de decisao[%d] - amostra altura = %d %d\n",i, amostra, altura);

        amostra_aleatoria(amostra, dataset, ale);
        arv->v_arv[i] = build_tree(ale, altura, 1);
    }

    libera_matriz(ale);
    return arv;
}

//prevê o valor esperado para cada decisionTree na random forest e pega o valor que mais aparece
double prever(RandomForest* node, Vetor row) {
    double maior=0;
    int i;
    Vetor pred;
    pred.v = (double*)malloc(node->n * sizeof(double));
    pred.n = node->n;
    for(i=0; i<node->n; i++) {
        pred.v[i] = predict(node->v_arv[i], row);
    }

    maior = contar_maior(pred);
    free(pred.v);
    return (double)(maior);
}

//calcula a porcentagem de acerto da Random Forest no dataset
double porcentagem_acerto(matriz* dataset, RandomForest* arv) {
    int i, k=0;
    Grupo *pred = matriz_to_grupo(dataset);
    for(i = 0; i < pred->n; i++) {
        double p=0, r=0;
        p = prever(arv, pred->v[i]);
        r = pred->v[i].v[pred->v[i].n - 1];
        if(p == r) {
            k++;
        }
    }
    desaloca_grupo(*pred);
    free(pred);

    return ( ((double)k) / (double)dataset->i);
}

void imprimir_radomforest(RandomForest* arv) {
    int i;
    for(i=0; i < arv->n; i++) {
        printf("\n-------Arvore de decisao %d--------\n", i);
        imprimir_arvore(arv->v_arv[i], 0);
    }
}

void desaloca_randomforest(RandomForest* arv) {
    int i;
    for(i=0; i < arv->n; i++) {
        desaloca_arvore(arv->v_arv[i]);
    }
    free(arv->v_arv);
    free(arv);
}