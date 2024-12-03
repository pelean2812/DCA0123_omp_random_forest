#include "arvore.h"
#include <omp.h>

//cria nó vazio
No* criar_no() {
    No* node;
    node = (No*)malloc(sizeof(No));
    node->index = 0;
    node->value = 0;
    node->grupos.n = 0;
    node->grupos.v = NULL;
    node->left = NULL;
    node->right = NULL;
    node->v_left = 0;
    node->v_right = 0;
    node->folha = 1;
    return node;
}

//calcula o gini_index, uma medição que retorna o quão bom é a divisão da arvore naquele ponto dos dados
double gini_index(Grupos grupos, Vetor classes) {
    double gini, score, p;
    unsigned int i, j, n_instances, tam;

    //Contar o numero de instâncias nos grupos
    n_instances = 0;

    for(i = 0; i < grupos.n; i++) {
        n_instances += grupos.v[i].n;
    }

    //Calcular o Gini index
    gini = 0;
    #pragma omp parallel for default (none)\
    shared(grupos, classes, n_instances, gini)\
    private(i, j, tam, score, p)
    for(i = 0; i < grupos.n; i++) {
        tam = grupos.v[i].n;
        if(tam == 0)
            continue;

        //Atribuir um score para grupo baseado na classe
        score = 0.0;
        for(j = 0; j < classes.n; j++) {
            unsigned int k, linhas = grupos.v[i].n;

            Vetor aux;
            aux.v = (double*)malloc(linhas * sizeof(double));
            aux.n = linhas;

            for(k = 0; k < linhas; k++) {
                unsigned int tam_linha = grupos.v[i].v[k].n;
                aux.v[k] = grupos.v[i].v[k].v[tam_linha - 1];
            }
            p = (contar(aux, classes.v[j])) / ((double)tam);
            score += p * p;
            free(aux.v);
        }

        #pragma omp critical
        gini += (1.0 - score) * ((double)tam / n_instances);
    }
    return gini;
}

//retorna uma divisão dos dados para depois testar na função get_split
Grupos test_split(unsigned int index, double value, matriz* dataset) {
    Grupo left, right;
    unsigned int l=0, r=0, k, p;
    left.n = right.n = 0;
    left.v = (Vetor*)malloc(dataset->i * sizeof(Vetor));
    right.v = (Vetor*)malloc(dataset->i * sizeof(Vetor));
    unsigned int i, j;

    #pragma omp parallel default(none) private(i, k)\ 
    shared(r, l, left, right, dataset, index, value) 
    {
        #pragma omp for
        for(i = 0; i < dataset->i; i++) {
            if(dataset->M[i][index] < value) {
                #pragma omp critical
                {
                    left.v[l].n = dataset->j;
                    left.v[l].v = (double*)malloc(dataset->j * sizeof(double));
                    for(k = 0; k < dataset->j; k++) {
                        left.v[l].v[k] = dataset->M[i][k];
                    }
                    l++;
                }
            } else {
                #pragma omp critical
                {
                    right.v[r].n = dataset->j;
                    right.v[r].v = (double*)malloc(dataset->j * sizeof(double));
                    for(k = 0; k < dataset->j; k++) {
                        right.v[r].v[k] = dataset->M[i][k];
                    }
                    r++;
                }
            }
        }
    }

    left.n = l;
    right.n = r;
    Grupos split;
    split.n = 2;
    split.v = (Grupo*)malloc(2 * sizeof(Grupo));
    split.v[0] = left;
    split.v[1] = right;

    return split;
}

//testa todas as possíveis divisões da árvore e retorna aquela com o menor gini index
No* get_split(matriz* dataset) {
    Vetor class_values;
    int i,j, index;
    class_values.v = (double*)malloc(dataset->i*sizeof(double));
    class_values.n = dataset->i;
    
    #pragma omp parallel for default(none)\
    shared(dataset, class_values) private(i)
    for(i=0; i < dataset->i; i++) {
        class_values.v[i] = dataset->M[i][(dataset->j)-1];
    }
    
    eleminar_repetido(&class_values);
    int b_index=999;
    double b_value = 999, b_score=999;
    double gini=0;
    Vetor aux;
    aux.n = 2;
    aux.v = (double*)malloc(2*sizeof(double));
    aux.v[0] = 0;
    aux.v[1] = 1;
    Grupos b_groups, groups;

    #pragma omp parallel for default(none)\
    shared(dataset, class_values, b_index, b_value, b_score, b_groups, aux)\
    private(i, j, index, groups, gini)
    for(index = 0; index < (dataset->j)-1; index++) {
        for(i=0; i < dataset->i; i++) {
            groups = test_split(index, dataset->M[i][index],dataset);
            gini = gini_index(groups, class_values);
            
            #pragma omp critical
            {
                if(gini < b_score) {
                    b_index = index;
                    b_value = dataset->M[i][index];
                    b_score = gini;
                    b_groups = groups;
                }
            } 
        }
    }
    No* no = criar_no();
    no->index = b_index;
    no->value = b_value;
    no->grupos.n = b_groups.n;
    no->grupos.v = b_groups.v;
    return no;
}

//retorna o resultado para tal divisão dos dados baseado em qual valor aparece mais
double to_terminal(Grupo g) {
    Vetor outcomes, aux;
    int i,j, last;
    outcomes.n = aux.n = g.n;
    outcomes.v = (double*)malloc(g.n * sizeof(double));
    aux.v = (double*)malloc(g.n * sizeof(double));
    for(i=0; i < g.n; i++) {
        last = (g.v[i].n) - 1;
        outcomes.v[i] = g.v[i].v[last];
        aux.v[i] = g.v[i].v[last];
    }

    eleminar_repetido(&outcomes);
    int maior = 0, cont=0;
    double res;
    for(i=0; i < outcomes.n; i++) {
        cont = contar(aux, outcomes.v[i]);
        if(cont > maior) {
            res = outcomes.v[i];
            maior = cont;
        }
    }

    free(aux.v);
    free(outcomes.v);

    return res;
}

//divide a árvore em nós até atingir a altura máxima ou o tamanho mínimo dos dados
void split(No* node, int max_depth, int min_size, int depth) {
    Grupo left, right, aux;
    int i, j;

    if(node->grupos.v != NULL) {
        left = node->grupos.v[0];
        right = node->grupos.v[1];
        node->grupos.v = NULL;
    } else {
        left.v = right.v = NULL;
        left.n = right.n = 0;
    }
    if((left.n == 0) || (right.n == 0)) {
        aux.n = left.n + right.n;
        aux.v = (Vetor*)malloc(aux.n * sizeof(Vetor));
        #pragma omp parallel for default(none)\
        private(i) shared(left, aux)
        for(i=0; i < left.n; i++) {
            aux.v[i] = left.v[i];
        }
        for(i=0, j=left.n; i < right.n; i++, j++) {
            aux.v[j] = right.v[i];
        }
        node->v_left = node->v_right = to_terminal(aux);
        node->folha = 1;
        free(aux.v);
        return;
    }
    if(depth >= max_depth) {
        node->v_left = to_terminal(left);
        node->v_right = to_terminal(right);
        node->folha = 1;
        return;
    }
#   pragma omp parallel default(none)\
    shared(left, right, node, max_depth, min_size, depth)
{
    //processar o filho esquerdo
    if(left.n <= min_size) {
        node->v_left = to_terminal(left);
        node->folha = 1;
    } else {
        matriz* m = grupo_to_matriz(left);
        node->folha = 0;
        node->left = get_split(m);
        libera_matriz(m);

        split(node->left, max_depth, min_size, depth+1);
    }
}
    desaloca_grupo(left);

#   pragma omp parallel default(none)\
    shared(left, right, node, max_depth, min_size, depth)
{
    //processar o filho direito
    if(right.n <= min_size) {
        node->folha=1;
        node->v_right = to_terminal(right);
    } else {
        matriz* m = grupo_to_matriz(right);
        node->folha = 0;
        node->right = get_split(m);
        libera_matriz(m);

        split(node->right, max_depth, min_size, depth+1);
    }
}
    desaloca_grupo(right);
}

//constroi a melhor árvore com os dados, altura e tamanho mínimo das divisões
No* build_tree(matriz* m, int max_depth, int min_size) {
    No* root = criar_no();
    root = get_split(m);
    split(root, max_depth, min_size, 1);
    return root;
}

//imprime a árvore
void imprimir_arvore(No* node, int depth) {
    int i;
    if(node->folha) {
        for(i=0; i < depth; i++) {
            printf("    ");
        }
        printf("[X%lu < %.3lf]\n", node->index+1, node->value);
        for(i=0; i < depth+1; i++) {
            printf("    ");
        }
        printf("[%lf]\n", node->v_left);
        for(i=0; i < depth+1; i++) {
            printf("    ");
        }
        printf("[%lf]\n", node->v_right);
    }
    else {
        for(i=0; i < depth; i++) {
            printf("    ");
        }
        printf("[X%lu < %.3lf]\n", node->index, node->value);
        if(node->left)
            imprimir_arvore(node->left, depth+1);
        if(node->right)
            imprimir_arvore(node->right, depth+1);
    }
}

//dado uma linha dos dados prevê o valor esperado
double predict(No* node, Vetor row) {
    if(row.v[node->index] < node->value) {
        if(node->folha) {
            return node->v_left;
        }
        else {
            if(node->left != NULL) {
                return predict(node->left, row);
            }
            else {
                return node->v_left;
            }
        }
    }
    else {
        if(node->folha) {
            return node->v_right;
        }
        else {
            if(node->right != NULL) {
                return predict(node->right, row);
            }
            else {
                return node->v_right;
            }
        }
    }
}

void desaloca_arvore(No* arv) {
    if(arv != NULL) {
        if(arv->folha) {
            desaloca_grupos(arv->grupos);
            free(arv);
        }
        else {
            desaloca_grupos(arv->grupos);
            desaloca_arvore(arv->left);
            desaloca_arvore(arv->right);
            free(arv);
        }
    }
}