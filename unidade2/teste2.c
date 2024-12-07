#include "matriz.h"
#include "arvore.h"
#include "Vetor.h"
#include "RandomForest.h"

int main() {
    //nome do arquivo dos dados
    char s[50] = "data/breast-cancer-wisconsin.data";
    TrainTest tt;
    matriz* m, *treino, *teste;
    Grupo* g;
    int i, j;
    srand(time(NULL));

    //coleta os dados e coloca em m
    m = coleta_dados(s);
    
    //separa dados de treino e dados de teste
    tt = train_test_split(m, 0.80);
    treino = tt.train;
    teste = tt.test;

    //cria random forest com 10 árvores de decisão, com amostra minima 20 e com altura máxima 8
    RandomForest* arvore=NULL;
    arvore = cria_random_forest(6, 20, 8, treino);

    //Imprime as árvores da Random Forest
    imprimir_radomforest(arvore);

    //calcula porcentagem de acerto
    double acerto = porcentagem_acerto(teste, arvore);
    printf("\nAcerto da Random Forest = %lf\n", acerto);

    //desaloca toda memoria alocada das arvores
    desaloca_randomforest(arvore);

    libera_matriz(m);
    libera_matriz(treino);
    libera_matriz(teste);

    return 0;
}
