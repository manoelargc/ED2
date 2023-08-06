#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*aula 12/05/23 - entrega pro dia 18/05 ATIVIDADE COM ARVORE BINARIA DE BUSCA
Manoela Resende 2210100235

2) inserir 10000 numeros (sem repetição) em uma arvore binária de busca, remover em 
grupos crescentes de 1.000 em 1.000, guardar em um vetor, o numero de comparacoes executadas
fazer gráfico tam do bloco x n de comparacoes.*/

#define TAM_INICIO 100000
#define TAM_GRUPO 1000

struct arv
{
    int val;
    struct arv *esq;
    struct arv *dir;
};
typedef struct arv Arv;

// Protótipos das funções
Arv *arv_cria(int v);
Arv *arv_insere(Arv *a, int v);
Arv *arv_remove(Arv *a, int v);
void liberar_arvore(Arv *a);

int main()
{
    int i, j, k;

    int numeros[TAM_INICIO];
    for (i = 0; i < TAM_INICIO; i++)
    {
        numeros[i] = i;
    }

    FILE *arquivo = fopen("comparacoes_questao2.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int num_grupos = TAM_INICIO / TAM_GRUPO;
    int *comparacoesVetor = malloc(num_grupos * sizeof(int));
    int *tamanhos = malloc(num_grupos * sizeof(int));

    Arv *arvore = NULL;
    for (i = 0; i < TAM_INICIO; i++)
    {
        arvore = arv_insere(arvore, numeros[i]);
    }

    int tamanho_atual = TAM_INICIO;
    for (i = 0; i < num_grupos; i++)
    {
        int comparacoes = 0;

        for (j = i * TAM_GRUPO; j < (i + 1) * TAM_GRUPO; j++)
        {
            arvore = arv_remove(arvore, numeros[j]);
            comparacoes += j;
        }

        comparacoesVetor[i] = comparacoes;
        tamanhos[i] = tamanho_atual;
        tamanho_atual -= TAM_GRUPO;
    }

    for (i = 0; i < num_grupos; i++)
    {
        fprintf(arquivo, "%d %f\n", tamanhos[i], comparacoesVetor[i] / (float)tamanhos[i]);
    }

    fclose(arquivo);
    free(comparacoesVetor);
    free(tamanhos);

    liberar_arvore(arvore);

    return 0;
}

Arv *arv_cria(int v)
{
    Arv *novaArvore = (Arv *)malloc(sizeof(Arv));
    novaArvore->val = v;
    novaArvore->esq = NULL;
    novaArvore->dir = NULL;
    return novaArvore;
}

Arv *arv_insere(Arv *a, int v)
{
    if (a == NULL)
    {
        a = arv_cria(v);
    }
    else if (v < a->val)
    {
        a->esq = arv_insere(a->esq, v);
    }
    else if (v > a->val)
    {
        a->dir = arv_insere(a->dir, v);
    }
    return a;
}

Arv *arv_remove(Arv *a, int v)
{
    if (a == NULL)
    {
        return NULL;
    }
    else if (v < a->val)
    {
        a->esq = arv_remove(a->esq, v);
    }
    else if (v > a->val)
    {
        a->dir = arv_remove(a->dir, v);
    }
    else
    {
        if (a->esq == NULL && a->dir == NULL)
        {
            free(a);
            return NULL;
        }
        else if (a->esq == NULL)
        {
            Arv *temp = a;
            a = a->dir;
            free(temp);
        }
        else if (a->dir == NULL)
        {
            Arv *temp = a;
            a = a->esq;
            free(temp);
        }
        else
        {
            Arv *temp = a->dir;
            while (temp->esq != NULL)
            {
                temp = temp->esq;
            }
            a->val = temp->val;
            a->dir = arv_remove(a->dir, temp->val);
        }
    }
    return a;
}

void liberar_arvore(Arv *a)
{
    if (a != NULL)
    {
        liberar_arvore(a->esq);
        liberar_arvore(a->dir);
        free(a);
    }
}
