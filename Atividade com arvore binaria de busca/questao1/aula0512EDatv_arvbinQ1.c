#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*aula 12/05/23 - entrega pro dia 18/05 ATIVIDADE COM ARVORE BINARIA DE BUSCA
Manoela Resende 2210100235

1a) criar vetores de 10 a 100000, embaralhar aleatoriamente, sem permitir sobreposições ou repetições dos valores
1b) inserir os valores de 1a, em ordem crescente de 100 em 100 elementos, e calcular a qtd de comparações feitas
e guardar o valor em um vetor.
1c) vet com a qtd de comparacoes em cada insercao, gravar esses dados em um arquivo e fazer um gráfico do
n de insercoes x tam do grupo de números inseridos.*/ 

#define TAM_INICIO 10
#define TAM_FINAL 100000

int i, j;//declaro como global pq no meu vscode nao compila declarando no for

struct arv //representa um nodo da arvore
{
    int val;
    struct arv *esq;
    struct arv *dir;
};
typedef struct arv Arv;

//prototipos das funcoes

Arv *arv_criavazia();
Arv *arv_cria(int v);
Arv *arv_insere(Arv *a, int v);
int contar_comparacoes(Arv *a, int v, int *comparacoes);
void liberar_arvore(Arv *a); 
void embaralhar_vetor(int *vet, int tamanho);
void percorre(Arv* a);


int main()
{
    srand(time(NULL));

    int numeros[TAM_FINAL]; //vetor pra armazenar os numeros que serao inseridos na arvore

    // preenche o vetor com valores sequenciais de 1 a 100000
    for (i = 0; i < TAM_FINAL; i++)
    {
        numeros[i] = i + TAM_INICIO;//cada posicao comeca com 10
    }

    // embaralha o vetor (QUESTAO 1A)
    embaralhar_vetor(numeros, TAM_FINAL);

    FILE *arquivo = fopen("comparacoes_questao1.txt", "w"); //abre arquivo em modo de escrita
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int incremento = 100;
    int *comparacoesVetor = malloc(TAM_FINAL * sizeof(int)); //guardar o numero de comparacoes p/ cada tamanho de grupo inseridos

    // insere os valores em ordem na árvore e registra as comparações em um arquivo (QUESTAO 1B)
    for (i = TAM_INICIO; i < TAM_FINAL; i += incremento)
    {
        //itera sobre tamanho do grupo de n. a serem inseridos
        Arv *arvore = arv_criavazia(); //uma arv vazia a cada tamanho
        int comparacoes = 0; //conta o n de comparacoes p/ cada valor inserido

        for (j = 0; j <= i; j++)
        {
            arvore = arv_insere(arvore, numeros[j]);
            contar_comparacoes(arvore, numeros[j], &comparacoes);
        }
        //i+incremento=altura
        comparacoesVetor[i] = comparacoes; //guarda aqui o numero

        fprintf(arquivo, "%d %f\n", i + incremento, comparacoes/(float)(i+incremento)); //escreve n de comparacoes e o tamanho do grupo *****N COMPARACOES DIVIDIDO PELO TAMANHO*****


    /*     printf("\ngraph G{\n");
        percorre(arvore);
        printf("}\n");*/
        liberar_arvore(arvore); 
    }




    fclose(arquivo);
    free(comparacoesVetor);

    return 0;
}

Arv *arv_criavazia()
{
    return NULL; //arvore vazia
}

    void percorre(Arv* a)
{
    if (a == NULL)
    {
        return;
    }
    if (a->esq != NULL)
    {
        printf("\"%c\" -- \"%c\"\n", a->val, a->esq->val);
        percorre(a->esq);
    }
    if (a->dir != NULL)
    {
        printf("\"%c\" -- \"%c\"\n", a->val, a->dir->val);
        percorre(a->dir);
    }
}

Arv *arv_cria(int v) //cria novo nodo de arvore
{
    Arv *novaArvore = (Arv *)malloc(sizeof(Arv));
    novaArvore->val = v;
    novaArvore->esq = NULL;
    novaArvore->dir = NULL;
    return novaArvore;

}

Arv *arv_insere(Arv *a, int v) //insere um valor numa arvore
{
    if (a == NULL)
    {
        a = arv_cria(v); //se tiver vazia, cria um novo nodo
    }
    else if (v < a->val)
    {
        a->esq = arv_insere(a->esq, v); //senao, o valor compara com o atual e chama recursivamente pra inserir o valor no filho
    }
    else if (v > a->val)
    {
        a->dir = arv_insere(a->dir, v);
    }
    return a; //retorna a raiz da arvore atualizada
}

int contar_comparacoes(Arv *a, int v, int *comparacoes)
{
    if (a == NULL)
    {
        return 0;
    }
    else if (v == a->val)
    {
        return 1;
    }
    else if (v < a->val)
    {
        (*comparacoes)++;
        return contar_comparacoes(a->esq, v, comparacoes);
    }
    else
    {
        (*comparacoes)++;
        return contar_comparacoes(a->dir, v, comparacoes);
    }
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

void embaralhar_vetor(int *vet, int tamanho) //embaralha de forma aleatoria os elementos do vetor (QUESTAO 1)
{
    for (i = tamanho - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}

