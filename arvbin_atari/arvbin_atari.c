#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256

typedef struct Jogo
{
    char nome[TAM];
    char developer[TAM];
    char ano[TAM];
    char genero[TAM];
    char path_img_capa[TAM];
    char path_img_tela[TAM];
    struct Jogo *esq;
    struct Jogo *dir;
    int altura;
} Jogo;

typedef unsigned int* colorattr;


int max(int a, int b);
Jogo *criarJogo(const char *nome, const char *developer, const char *ano, const char *genero, const char *path_img_capa, const char *path_img_tela);
Jogo *inserirJogo(Jogo *raiz, Jogo *jogo);
Jogo *rotacaoDireita(Jogo *no);
Jogo *rotacaoEsquerda(Jogo *no);
Jogo *rotacaoDuplaDireita(Jogo *no);
Jogo *rotacaoDuplaEsquerda(Jogo *no);
int altura(Jogo *no);
int fatorBalanceamento(Jogo *no);
Jogo *buscarJogo(Jogo *raiz, const char *nome, int *comparacoes);
Jogo *encontrarMinimo(Jogo *raiz);
Jogo *removerJogo(Jogo *raiz, const char *nome);
void imprimirJogo(Jogo *jogo);
void exibirJogos(Jogo *raiz);
void liberarArvore(Jogo *raiz);
void gravarDados(Jogo *raiz, FILE *arquivo);
void escreveGrafo(Jogo *no, FILE *arquivo, colorattr nodecor[], colorattr fontcor[], int *contador);
void gerarGraphviz(Jogo *raiz, const char *nomeArquivo);
void gravarArquivoGraphviz(Jogo *raiz);

//------------
// execução
//------------

int main()
{
    Jogo *arvoreJogos = NULL;

    //----------------------------------------------------------------------
    //============ CARREGANDO OS JOGOS JÁ EXISTENTES NO ATARI 2600 ============
    //----------------------------------------------------------------------

    FILE *arquivo = fopen("lista_jogos.txt", "r"); // arquivo com a lista dos jogos das tabelas do link
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[TAM];
    char nome[TAM];
    char developer[TAM];
    char ano[TAM];
    char genero[TAM];
    char path_img_capa[TAM];
    char path_img_tela[TAM];
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        sscanf(linha, "%s %s %s %s %s %s\n", nome, developer, ano, genero, path_img_capa, path_img_tela);
        fgets(linha, sizeof(linha), arquivo);

        Jogo *novoJogo = criarJogo(nome, developer, ano, genero, path_img_capa, path_img_tela); // criando com as informações da tabela

        arvoreJogos = inserirJogo(arvoreJogos, novoJogo); // inserindo na arvore
    }
    fclose(arquivo);

    int opcao; // para o switch case

    int comparacoes = 0;

    //----------------------------------------------------------------------
    //============================== INTERFACE ================================
    //----------------------------------------------------------------------

    do
    {
        printf("========================| MENU |========================\n");
        printf("(1) Cadastrar novo jogo\n");
        printf("(2) Editar jogo\n");
        printf("(3) Excluir jogo\n");
        printf("(4) Consultar jogo por nome\n");
        printf("(5) Biblioteca de jogos\n");
        printf("(6) Gravar base de dados em arquivo\n");
        printf("(7) Gerar arquivo Graphviz\n"); // Opção adicionada
        printf("(0) Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            //----------------------------------------------------------------------
        case 1: // CADASTRO DO NOVO JOGO
            //----------------------------------------------------------------------

            printf("\n=====================| NOVO JOGO |=====================\n");

            char nome[TAM];
            char developer[TAM];
            char ano[TAM];
            char genero[TAM];
            char path_img_capa[TAM];
            char path_img_tela[TAM];

            printf("Digite o nome do jogo: ");
            scanf("%s", nome);
            printf("Digite o desenvolvedor do jogo: ");
            scanf("%s", developer);
            printf("Digite o ano de lancamento do jogo: ");
            scanf("%s", ano);
            printf("Digite o genero do jogo: ");
            scanf("%s", genero);
            printf("Digite o path da imagem da capa do jogo: ");
            scanf("%s", path_img_capa);
            printf("Digite o path da imagem da tela do jogo: ");
            scanf("%s", path_img_tela);

            Jogo *novoJogo = criarJogo(nome, developer, ano, genero, path_img_capa, path_img_tela);

            arvoreJogos = inserirJogo(arvoreJogos, novoJogo);

            printf("Jogo cadastrado com sucesso!\n");
            printf("\n========================================================\n");
            break;

            //----------------------------------------------------------------------
        case 2: // ATUALIZAR JOGO JÁ EXISTENTE / EDITAR
            //----------------------------------------------------------------------
            printf("\n=====================| EDITOR |=====================\n");
            printf("Digite o nome do jogo a ser atualizado: ");
            scanf("%s", nome);
            Jogo *jogo = buscarJogo(arvoreJogos, nome, &comparacoes);
            if (jogo == NULL)
            {
                printf("Jogo com nome %s nao encontrado!\n", nome);
                printf("\n========================================================\n");
                break;
            }

            char novoNome[TAM];
            char novoDeveloper[TAM];
            char novoAno[TAM];
            char novoGenero[TAM];
            char novoPathImagemCapa[TAM];
            char novoPathImagemTela[TAM];

            printf("Digite o novo nome do jogo: ");
            scanf("%s", novoNome);
            printf("Digite o novo desenvolvedor do jogo: ");
            scanf("%s", novoDeveloper);
            printf("Digite o novo ano de lancamento do jogo: ");
            scanf("%s", novoAno);
            printf("Digite o novo genero do jogo: ");
            scanf("%s", novoGenero);
            printf("Digite o novo path da imagem da capa do jogo: ");
            scanf("%s", novoPathImagemCapa);
            printf("Digite o novo path da imagem da tela do jogo: ");
            scanf("%s", novoPathImagemTela);

            // atualiza as informacões do jogo com os novos valores
            strcpy(jogo->nome, novoNome);
            strcpy(jogo->developer, novoDeveloper);
            strcpy(jogo->ano, novoAno);
            strcpy(jogo->genero, novoGenero);
            strcpy(jogo->path_img_capa, novoPathImagemCapa);
            strcpy(jogo->path_img_tela, novoPathImagemTela);

            printf("Jogo atualizado com sucesso!\n");
            printf("\n========================================================\n");
            break;

            //----------------------------------------------------------------------
        case 3: // REMOVER JOGO JÁ EXISTENTE
            //----------------------------------------------------------------------

            printf("\n====================| EXCLUIR JOGO |====================\n");
            printf("Digite o nome do jogo a ser removido: ");
            scanf("%s", nome);
            Jogo *jogoRemover = buscarJogo(arvoreJogos, nome, &comparacoes);
            if (jogoRemover == NULL)
            {
                printf("Jogo com nome %s nao encontrado!\n", nome);
                printf("\n========================================================\n");
                break;
            }

            arvoreJogos = removerJogo(arvoreJogos, nome);
            printf("Jogo removido com sucesso!\n");
            printf("\n========================================================\n");
            break;

            //----------------------------------------------------------------------
        case 4: // BUSCAR OS JOGOS POR NOME
            //----------------------------------------------------------------------

            printf("\n=====================| PESQUISAR |=====================\n");
            printf("Digite o nome do jogo: ");
            scanf("%s", nome);
            comparacoes = 0;
            Jogo *game = buscarJogo(arvoreJogos, nome, &comparacoes);
            if (game != NULL)
            {
                imprimirJogo(game);
                printf("\nNumero de comparacoes: %d\n", comparacoes);

                // abre a imagem da capa do jogo selecionado
                char str_arg[TAM];
                strcpy(str_arg, "explorer ");
                strcat(str_arg, game->path_img_capa);
                system(str_arg);

                // abre a imagem da tela do jogo selecionado
                strcpy(str_arg, "explorer ");
                strcat(str_arg, game->path_img_tela);
                system(str_arg);

                // jogos que já coloquei a imagem de teste:
                // combat, 32-in-1, pitfall, river-raid, asterix, zaxxon e mario-bros
            }
            else
            {
                printf("Jogo nao encontrado!\n");
            }
            printf("\n========================================================\n");
            break;

            //----------------------------------------------------------------------
        case 5: // MOSTRAR TODOS OS JOGOS
            //----------------------------------------------------------------------
            exibirJogos(arvoreJogos);
            break;

            //----------------------------------------------------------------------
        case 6: // GRAVAR BASE DE DADOS NUM ARQUIVO
            //----------------------------------------------------------------------

            printf("\n=====================| SALVAR ARQUIVO |=====================\n");

            printf("Digite o nome do arquivo para gravar os dados\n(acrescente '.txt' apos o nome esolhido): "); // pra ser arquivo de texto
            char nomeArquivo[TAM];
            scanf("%s", nomeArquivo); // usuario escolhe o nome

            FILE *arquivo = fopen(nomeArquivo, "w"); // abre o arquivo para escrita
            if (arquivo == NULL)
            {
                printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
                printf("\n========================================================\n");
                break;
            }

            gravarDados(arvoreJogos, arquivo);

            fclose(arquivo);
            printf("Dados salvos no arquivo %s com sucesso!\n", nomeArquivo);
            printf("\n========================================================\n");
            // break;
            break;

            //----------------------------------------------------------------------
        case 7: // CRIAR UM ARQUIVO DO GRAFO DA ARVORE NO GRAPHVIZ
            //----------------------------------------------------------------------
            printf("\n=====================| GERAR GRAFO |=====================\n");
            gravarArquivoGraphviz(arvoreJogos);
            printf("\n========================================================\n");
            break;

        case 0:
            printf("Encerrando o programa.\n");
            printf("\n========================================================\n");

            break;
        default:
            printf("Opcao invalida!\n");
            printf("\n========================================================\n");
            break;
        }

        printf("\n");

    } while (opcao != 0);

    // Liberar a memoria alocada pela arvore
    liberarArvore(arvoreJogos);

    return 0;
}

//----------------------------------------------------------------------
//============================== FUNÇÕES ==================================
//----------------------------------------------------------------------


//---------------------------------------
// cria um jogo 
//---------------------------------------
Jogo *criarJogo(const char *nome, const char *developer, const char *ano, const char *genero, const char *path_img_capa, const char *path_img_tela)
{
    Jogo *novoJogo = (Jogo *)malloc(sizeof(Jogo));
    strcpy(novoJogo->nome, nome);
    strcpy(novoJogo->developer, developer);
    strcpy(novoJogo->ano, ano);
    strcpy(novoJogo->genero, genero);
    strcpy(novoJogo->path_img_capa, path_img_capa);
    strcpy(novoJogo->path_img_tela, path_img_tela);
    novoJogo->esq = NULL;
    novoJogo->dir = NULL;
    novoJogo->altura = 1;
    return novoJogo;
}

//---------------------------------------
// calcula a altura de um nodo na arvore
//---------------------------------------
int altura(Jogo *no)
{
    if (no == NULL)
        return 0;
    return no->altura;
}

//---------------------------------------
// função auxiliar que retorna o maximo entre 2 valores
//---------------------------------------
int max(int a, int b)
{
    return (a > b) ? a : b;
}

//---------------------------------------
// calcula o fator de balanceamento em cada nodo da arvore
//---------------------------------------
int fatorBalanceamento(Jogo *no)
{
    if (no == NULL)
        return 0;
    return altura(no->esq) - altura(no->dir);
}

//---------------------------------------
// realiza a rotacao simples a direita em um nodo
//---------------------------------------
Jogo *rotacaoDireita(Jogo *no)
{
    Jogo *esq = no->esq;
    Jogo *esqDir = esq->dir;

    // faz a rotacao
    esq->dir = no;
    no->esq = esqDir;

    // atualiza as alturas dos nodos afetados pela rotacao
    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    esq->altura = 1 + max(altura(esq->esq), altura(esq->dir));

    return esq; // retorna o novo nodo raiz
}

//---------------------------------------
// rotacao simples a esquerda do nodo
//---------------------------------------
Jogo *rotacaoEsquerda(Jogo *no)
{
    Jogo *dir = no->dir;
    Jogo *dirEsq = dir->esq;

    // faz a rotacao
    dir->esq = no;
    no->dir = dirEsq;

    // atualiza as alturas dos nodos afetados pela rotacao
    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    dir->altura = 1 + max(altura(dir->esq), altura(dir->dir));

    return dir; // retorna o novo nodo raiz
}

//---------------------------------------
// realiza rotacao dupla a direita do nodo
//---------------------------------------
Jogo *rotacaoDuplaDireita(Jogo *no)
{
    // faz a rotacao dupla a direita
    no->esq = rotacaoEsquerda(no->esq);
    return rotacaoDireita(no); // retorna o novo nodo raiz
}

//---------------------------------------
// rotacao dupla a esquerda do nodo
//---------------------------------------
Jogo *rotacaoDuplaEsquerda(Jogo *no)
{
    // faz a rotacao dupla a esquerda
    no->dir = rotacaoDireita(no->dir);
    return rotacaoEsquerda(no); // retorna o novo nodo raiz
}


//---------------------------------------
// insere um jogo na arvore AVL
//---------------------------------------

Jogo *inserirJogo(Jogo *raiz, Jogo *jogo)
{
    if (raiz == NULL)
        return jogo;  // se a raiz for nula, o jogo é inserido como raiz da arvore

    if (strcmp(jogo->nome, raiz->nome) < 0)
        raiz->esq = inserirJogo(raiz->esq, jogo);  // se o nome do jogo for menor que o nome do nodo atual, o jogo é inserido na subarvore esquerda
    else if (strcmp(jogo->nome, raiz->nome) > 0)
        raiz->dir = inserirJogo(raiz->dir, jogo);  // se for maior que o nome do atual, é inserido na subarvore direita
    else
        return raiz;  // se for igual ao nome do nodo atual, o jogo já existe na arvore e retorna a raiz sem fazer mudancas

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));  // atualiza a altura do nodo raiz

    int balanceamento = fatorBalanceamento(raiz);  // calcula o fator de balanceamento do nodo raiz

    // Realiza as rotações necessárias para balancear a arvore
    if (balanceamento > 1 && strcmp(jogo->nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz); 

    if (balanceamento < -1 && strcmp(jogo->nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);  

    if (balanceamento > 1 && strcmp(jogo->nome, raiz->esq->nome) > 0)
        return rotacaoDuplaDireita(raiz);  

    if (balanceamento < -1 && strcmp(jogo->nome, raiz->dir->nome) < 0)
        return rotacaoDuplaEsquerda(raiz);  

    return raiz;  // retorna a raiz atualizada
}


//---------------------------------------
// buscar um jogo na arvore por nome
//---------------------------------------
Jogo *buscarJogo(Jogo *raiz, const char *nome, int *comparacoes)
{
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    (*comparacoes)++;// incrementa pra depois retornar o numero de comparações feitas

    if (strcmp(nome, raiz->nome) < 0)
        return buscarJogo(raiz->esq, nome, comparacoes);
    else
        return buscarJogo(raiz->dir, nome, comparacoes);
}

Jogo *encontrarMinimo(Jogo *raiz)
{
    if (raiz == NULL)
    {
        return NULL;  // nao tem nenhum elemento
    }

    while (raiz->esq != NULL)
    {
        raiz = raiz->esq;  // percorre a arvore pela subarvore esquerda até encontrar o nodo mais à esquerda
    }

    return raiz;  // Retorna o nodo mais à esquerda, que é o nodo com o valor mínimo
}

//---------------------------------------
// remover um jogo da arvore
//---------------------------------------
Jogo *removerJogo(Jogo *raiz, const char *nome)
{
    if (raiz == NULL)
    {
        return NULL;
    }

    int comparacao = strcmp(nome, raiz->nome);
    if (comparacao < 0)
    {
        raiz->esq = removerJogo(raiz->esq, nome);
    }
    else if (comparacao > 0)
    {
        raiz->dir = removerJogo(raiz->dir, nome);
    }
    else
    {
        // Caso 1: O jogo nao possui filhos
        if (raiz->esq == NULL && raiz->dir == NULL)
        {
            free(raiz);
            raiz = NULL;
        }
        // Caso 2: O jogo possui apenas um filho a direita
        else if (raiz->esq == NULL)
        {
            Jogo *temp = raiz;
            raiz = raiz->dir;
            free(temp);
        }
        // Caso 3: O jogo possui apenas um filho a esq
        else if (raiz->dir == NULL)
        {
            Jogo *temp = raiz;
            raiz = raiz->esq;
            free(temp);
        }
        // Caso 8: O jogo possui dois filhos
        else
        {
            Jogo *temp = encontrarMinimo(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->dir = removerJogo(raiz->dir, temp->nome);
        }
    }

    return raiz;
}

//---------------------------------------
// imprimir os dados de um jogo
//---------------------------------------
void imprimirJogo(Jogo *jogo)
{
    printf("\n====================| %s |=====================\n", jogo->nome);
    printf("Nome: %s\n", jogo->nome);
    printf("Desenvolvido por: %s\n", jogo->developer);
    printf("Ano de Lancamento: %s\n", jogo->ano);
    printf("Genero: %s\n", jogo->genero);
    printf("Capa do Jogo: %s\n", jogo->path_img_capa);
    printf("Tela do Jogo: %s\n", jogo->path_img_tela);
    printf("\n========================================================\n");
}

//---------------------------------------
// mostrar todos os jogos em ordem
//---------------------------------------
void exibirJogos(Jogo *raiz)
{
    if (raiz != NULL)
    {
        exibirJogos(raiz->esq);
        imprimirJogo(raiz);
        printf("--------------------------------------------------------\n");
        exibirJogos(raiz->dir);
    }
}

//---------------------------------------
// liberar memoria alocada da arvore
//---------------------------------------
void liberarArvore(Jogo *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

//---------------------------------------
// gravar dados num arquivo
//---------------------------------------
void gravarDados(Jogo *raiz, FILE *arquivo)
{
    if (raiz != NULL)
    {
        gravarDados(raiz->esq, arquivo);

        fprintf(arquivo, "\n====================| %s |=====================\n", raiz->nome);
        fprintf(arquivo, "Nome: %s\n", raiz->nome);
        fprintf(arquivo, "Desenvolvido por: %s\n", raiz->developer);
        fprintf(arquivo, "Ano de Lancamento: %s\n", raiz->ano);
        fprintf(arquivo, "Genero: %s\n", raiz->genero);
        fprintf(arquivo, "Capa do Jogo: %s\n", raiz->path_img_capa);
        fprintf(arquivo, "Tela do Jogo: %s\n", raiz->path_img_tela);
        fprintf(arquivo, "\n========================================================\n");

        gravarDados(raiz->dir, arquivo);
    }
}


//---------------------------------------
// colocar cor em cada nodo
//---------------------------------------
void setnodecolor(colorattr nodecor[], colorattr fontcor[], int i, const char* nome) {
    unsigned long hash = 5381;
    int c;

    while ((c = *nome++)) {
        hash = ((hash << 5) + hash) + c; // Função de hash djb2
    }

    int r = (hash & 0xFF0000) >> 16;  // Componente vermelho (bits 16-23)
    int g = (hash & 0x00FF00) >> 8;   // Componente verde (bits 8-15)
    int b = hash & 0x0000FF;          // Componente azul (bits 0-7)
    
    // calcula a cor do nodo baseada nos valores r, g e b
    nodecor[i] = b | (g << 8) | (r << 16);
    
    // calcula a luminosidade da cor usando a fórmula da média
    int luma = (r + g + b) / 3;
    
    // Define a cor da fonte do nodo como branco (255) ou preto (0) dependendo da luminosidade
    fontcor[i] = luma > 127 ? 0x000000 : 0xFFFFFF;
}

//---------------------------------------
// função auxiliar para gerar o grafo
//---------------------------------------
void escreveGrafo(Jogo *no, FILE *arquivo, colorattr nodecor[], colorattr fontcor[], int *contador) {
    if (no == NULL)
        return;

    int i = (*contador)++;

    // Chama a função setnodecolor para calcular a cor do nodo com base no nome
    setnodecolor(nodecor, fontcor, i, no->nome);
    
    // Escreve o nodo com a cor calculada
    fprintf(arquivo, "\"%s\" [label=\"%s\", style=filled, fillcolor=\"#%06X\", fontcolor=\"#%06X\"];\n", no->nome, no->nome, nodecor[i], fontcor[i]);

    if (no->esq != NULL) {
        fprintf(arquivo, "\"%s\" -> \"%s\";\n", no->nome, no->esq->nome);
        escreveGrafo(no->esq, arquivo, nodecor, fontcor, contador);
    }

    if (no->dir != NULL) {
        fprintf(arquivo, "\"%s\" -> \"%s\";\n", no->nome, no->dir->nome);
        escreveGrafo(no->dir, arquivo, nodecor, fontcor, contador);
    }
}

//---------------------------------------
// função principal pra gerar o grafo no graphviz 
//---------------------------------------
void gerarGraphviz(Jogo *raiz, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    fprintf(arquivo, "digraph ArvoreJogos { rankdir=\"LR\"\n");

    colorattr nodecor[100]; // vetor para armazenar as cores dos nodos
    colorattr fontcor[100]; // vetor para armazenar as cores da fonte dos nodos
    int contador = 0;

    escreveGrafo(raiz, arquivo, nodecor, fontcor, &contador);

    fprintf(arquivo, "}\n");

    fclose(arquivo);
    printf("Arquivo Graphviz gerado com sucesso: %s\n", nomeArquivo);
}

//---------------------------------------
// grava o arquivo do grafo em .dot e .png
//---------------------------------------
void gravarArquivoGraphviz(Jogo *raiz)
{
    char nomeArquivo[256];
    printf("Digite o nome do arquivo para gerar o Graphviz (acrescente '.dot' ao nome): ");
    scanf("%s", nomeArquivo);
    gerarGraphviz(raiz, nomeArquivo);

    // Remove a extensão .dot do nome do arquivo
    char nomeArquivoPNG[256];
    strncpy(nomeArquivoPNG, nomeArquivo, strlen(nomeArquivo) - 4);
    nomeArquivoPNG[strlen(nomeArquivo) - 4] = '\0';

    // Gera a imagem PNG a partir do arquivo DOT
    char comando[100];
    sprintf(comando, "dot -Tpng %s -o %s.png", nomeArquivo, nomeArquivoPNG);
    int resultado = system(comando);

    if (resultado == 0)
    {
        printf("Imagem PNG gerada com sucesso: %s.png\n", nomeArquivoPNG);
    }
    else
    {
        printf("Erro ao gerar imagem PNG.\n");
    }
}
