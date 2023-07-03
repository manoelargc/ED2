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
void gerarGraphvizRecursivo(Jogo *no, FILE *arquivo);
void gerarGraphviz(Jogo *raiz, const char *nomeArquivo);
void gerarArquivoGraphviz(Jogo *raiz);

//------------
// execução
//------------

int main()
{
    Jogo *arvoreJogos = NULL;

    //-------------------------------------------------------------------------
    //============ CARREGANDO OS JOGOS JÁ EXISTENTES NO ATARI 2600 ============
    //-------------------------------------------------------------------------

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

    //-------------------------------------------------------------------------
    //============================== INTERFACE ================================
    //-------------------------------------------------------------------------

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
            //-------------------------------------------------------------------------
        case 1: // CADASTRO DO NOVO JOGO
            //-------------------------------------------------------------------------

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

            //-------------------------------------------------------------------------
        case 2: // ATUALIZAR JOGO JÁ EXISTENTE / EDITAR
            //-------------------------------------------------------------------------
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

            //-------------------------------------------------------------------------
        case 3: // REMOVER JOGO JÁ EXISTENTE
            //-------------------------------------------------------------------------

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

            //-------------------------------------------------------------------------
        case 4: // BUSCAR OS JOGOS POR NOME
            //-------------------------------------------------------------------------

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

            //-------------------------------------------------------------------------
        case 5: // MOSTRAR TODOS OS JOGOS
            //-------------------------------------------------------------------------
            exibirJogos(arvoreJogos);
            break;

            //-------------------------------------------------------------------------
        case 6: // GRAVAR BASE DE DADOS NUM ARQUIVO
            //-------------------------------------------------------------------------

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

            //-------------------------------------------------------------------------
        case 7: // CRIAR UM ARQUIVO DO GRAFO DA ARVORE NO GRAPHVIZ
            //-------------------------------------------------------------------------
            gerarArquivoGraphviz(arvoreJogos);
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

//-------------------------------------------------------------------------
//============================== FUNÇÕES ==================================
//-------------------------------------------------------------------------

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

int altura(Jogo *no)
{
    if (no == NULL)
        return 0;
    return no->altura;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int fatorBalanceamento(Jogo *no)
{
    if (no == NULL)
        return 0;
    return altura(no->esq) - altura(no->dir);
}

Jogo *rotacaoDireita(Jogo *no)
{
    Jogo *esq = no->esq;
    Jogo *esqDir = esq->dir;

    esq->dir = no;
    no->esq = esqDir;

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    esq->altura = 1 + max(altura(esq->esq), altura(esq->dir));

    return esq;
}

Jogo *rotacaoEsquerda(Jogo *no)
{
    Jogo *dir = no->dir;
    Jogo *dirEsq = dir->esq;

    dir->esq = no;
    no->dir = dirEsq;

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    dir->altura = 1 + max(altura(dir->esq), altura(dir->dir));

    return dir;
}

Jogo *rotacaoDuplaDireita(Jogo *no)
{
    no->esq = rotacaoEsquerda(no->esq);
    return rotacaoDireita(no);
}

Jogo *rotacaoDuplaEsquerda(Jogo *no)
{
    no->dir = rotacaoDireita(no->dir);
    return rotacaoEsquerda(no);
}

// Função para inserir um jogo na árvore AVL
// Atualize a função para retornar o número de comparações feitas
Jogo *inserirJogo(Jogo *raiz, Jogo *jogo)
{
    if (raiz == NULL)
        return jogo;

    if (strcmp(jogo->nome, raiz->nome) < 0)
        raiz->esq = inserirJogo(raiz->esq, jogo);
    else if (strcmp(jogo->nome, raiz->nome) > 0)
        raiz->dir = inserirJogo(raiz->dir, jogo);
    else
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int balanceamento = fatorBalanceamento(raiz);

    if (balanceamento > 1 && strcmp(jogo->nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz);

    if (balanceamento < -1 && strcmp(jogo->nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);

    if (balanceamento > 1 && strcmp(jogo->nome, raiz->esq->nome) > 0)
        return rotacaoDuplaDireita(raiz);

    if (balanceamento < -1 && strcmp(jogo->nome, raiz->dir->nome) < 0)
        return rotacaoDuplaEsquerda(raiz);

    return raiz;
}

//---------------------------------------
// buscar um jogo na arvore por nome
//------------------------------------------
Jogo *buscarJogo(Jogo *raiz, const char *nome, int *comparacoes)
{
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    (*comparacoes)++;

    if (strcmp(nome, raiz->nome) < 0)
        return buscarJogo(raiz->esq, nome, comparacoes);
    else
        return buscarJogo(raiz->dir, nome, comparacoes);
}

Jogo *encontrarMinimo(Jogo *raiz)
{
    if (raiz == NULL)
    {
        return NULL;
    }

    while (raiz->esq != NULL)
    {
        raiz = raiz->esq;
    }

    return raiz;
}

//---------------------------------------
// remover um jogo da árvore
//------------------------------------------
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
        // Caso 2: O jogo possui apenas um filho à direita
        else if (raiz->esq == NULL)
        {
            Jogo *temp = raiz;
            raiz = raiz->dir;
            free(temp);
        }
        // Caso 3: O jogo possui apenas um filho à esquerda
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
//------------------------------------------
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
//------------------------------------------
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
// liberar memoria alocada da árvore
//------------------------------------------
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
//------------------------------------------
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

// Função auxiliar para gerar o conteúdo Graphviz recursivamente
void gerarGraphvizRecursivo(Jogo *no, FILE *arquivo)
{
    if (no == NULL)
        return;

    // Escreve o nó atual no arquivo com um formato especial
    fprintf(arquivo, "\"%s\" [label=\"%s\", style=filled, fillcolor=\"#FFFF00\"];\n", no->nome, no->nome);

    // Chamada recursiva para os nós filhos
    gerarGraphvizRecursivo(no->esq, arquivo);
    gerarGraphvizRecursivo(no->dir, arquivo);

    // Escreve as arestas para os nós filhos
    if (no->esq != NULL)
        fprintf(arquivo, "\"%s\" -> \"%s\";\n", no->nome, no->esq->nome);
    if (no->dir != NULL)
        fprintf(arquivo, "\"%s\" -> \"%s\";\n", no->nome, no->dir->nome);
}

// Função para gerar o arquivo Graphviz
void gerarGraphviz(Jogo *raiz, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    // Escreve o cabeçalho do arquivo Graphviz
    fprintf(arquivo, "digraph ArvoreJogos {\n");

    // Chama a função auxiliar recursiva para gerar o conteúdo
    gerarGraphvizRecursivo(raiz, arquivo);

    // Escreve o rodapé do arquivo Graphviz
    fprintf(arquivo, "}\n");

    fclose(arquivo);
    printf("Arquivo Graphviz gerado com sucesso: %s\n", nomeArquivo);
}

// Função para gerar um arquivo Graphviz com nós e nomes usando cores
void gerarArquivoGraphviz(Jogo *raiz)
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