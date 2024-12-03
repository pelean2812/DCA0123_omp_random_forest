#include "Vetor.h"

//imprime struct vetor
void imprimir_vetor(Vetor v) {
    int i, j;
    printf("[");
    for(i=0; i < v.n; i++) {
        printf("%lf", v.v[i]);
        if(i != v.n - 1)
            printf(", ");
    }
    printf("]\n");
}

//imprime a struct grupos
void imprimir_grupos(Grupos g) {
    int i, j, k;
    printf("(");
    for(i=0; i < g.n; i++) {
        printf("[");
        for(j=0; j < g.v[i].n; j++) {
            printf("[");
            for(k=0; k < g.v[i].v[j].n; k++) {
                printf("%lf", g.v[i].v[j].v[k]);
                if(k != g.v[i].v[j].n - 1)
                    printf(", ");
            }
            printf("]");
            if(j != g.v[i].n - 1)
                printf(", ");
        }
        printf("]");
        if(i != g.n - 1)
            printf(", ");
    }
    printf(")\n");
}


//conta o tanto de valores "a" no vetor "vet"
unsigned int contar(Vetor vet, double a) {
    unsigned int n, i;
    n=0;
    for(i=0; i < vet.n; i++) {
        if(vet.v[i] == a) {
            n++;
        }
    }
    return n;
}

//implementação do quicksort
void quicksort(double values[], int began, int end) {
	int i, j, pivo;
    double aux;
	i = began;
	j = end-1;
	pivo = values[(began + end) / 2];
	while(i <= j)
	{
		while(values[i] < pivo && i < end)
		{
			i++;
		}
		while(values[j] > pivo && j > began)
		{
			j--;
		}
		if(i <= j)
		{
			aux = values[i];
			values[i] = values[j];
			values[j] = aux;
			i++;
			j--;
		}
	}
	if(j > began)
		quicksort(values, began, j+1);
	if(i < end)
		quicksort(values, i, end);
}

//elimina valores repetidos e ordena o vetor
void eleminar_repetido(Vetor* vet) {
    int i=0,j=0;
    double aux[100];
    Vetor res;
    res.n=0;
    quicksort(vet->v, 0, vet->n);
    for(i = 0; i < vet->n; i++) {
        j = i+1;
        if(j < vet->n && vet->v[i] == vet->v[j]) {
            aux[res.n] = vet->v[i];
            res.n++;
            while(j < vet->n && vet->v[i] == vet->v[j])
                j++;
            i = j;
        }
    }
    //free(vet->v);
    vet->v = (double*)malloc(res.n * sizeof(double));
    vet->n = res.n;
    for(i = 0; i < res.n; i++) {
        vet->v[i] = aux[i];
    }
}

//converte a struct "grupo" para a struct "matriz"
matriz* grupo_to_matriz(Grupo g) {
    int i, j, maior=0;
    
    matriz* m;
    m = (matriz*)malloc(sizeof(matriz));
    
    m->i = g.n;
    for(i = 0; i < g.n; i++) {
        if(g.v[i].n > maior) {
            maior = g.v[i].n;
            m->j = maior;
        }
        for(j=0; j < g.v[i].n; j++) {
            m->M[i][j] = g.v[i].v[j];
        }
    }
    return m;
}

//converte a struct matriz para a struct grupo
Grupo* matriz_to_grupo(matriz* m) {
    int i, j;
    Grupo* g = (Grupo*)malloc(sizeof(Grupo));

    g->n = m->i;
    g->v = (Vetor*)malloc(g->n * sizeof(Vetor));

    for(i=0; i < g->n; i++) {
        g->v[i].n = m->j;
        g->v[i].v = (double*)malloc(m->j * sizeof(double));
        for(j=0;j < g->v[i].n; j++) {
            g->v[i].v[j] = m->M[i][j];
        }  
    }

    return g;
}

//conta qual é o valor que mais aparece no vetor vet
double contar_maior(Vetor vet) {
    int i, j;
    Vetor aux;
    aux.n = vet.n;
    aux.v = (double*)malloc(aux.n * sizeof(double));

    for(i=0; i < aux.n; i++) {
        aux.v[i] = vet.v[i];
    }

    eleminar_repetido(&vet);
    int maior = 0, cont=0;
    double res;
    for(i=0; i < vet.n; i++) {
        cont = contar(aux, vet.v[i]);
        if(cont > maior) {
            res = vet.v[i];
            maior = cont;
        }
    }

    free(aux.v);

    return res;
}

//desaloca Vetor
void desaloca_vetor(Vetor vet) {
    if(vet.v == NULL)
        return;
    free(vet.v);
}

//desaloca Grupo
void desaloca_grupo(Grupo g) {
    int i;
    if(g.v == NULL)
        return;
    for(i=0; i < g.n; i++) {
        desaloca_vetor(g.v[i]);
    }
    free(g.v);
}

//desaloca Grupos
void desaloca_grupos(Grupos g) {
    int i;
    if(g.v == NULL)
        return;
    for(i=0; i < g.n; i++) {
        desaloca_grupo(g.v[i]);
    }
    free(g.v);
}