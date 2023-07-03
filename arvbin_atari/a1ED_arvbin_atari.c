#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256

//-------------------------------------------------------------------------------
/*AVALIAÇÃO COM ARVORE BINARIA DE BUSCA | ED2 2023.1 Manoela Resende 2210100235
//-------------------------------------------------------------------------------


Utilizando árvores binárias de busca e structs em C, crie um programa para realizar o cadastro e a consulta
de itens de uma coleção de jogos. O seu programa deverá ser capaz de permitir ao usuário:

- o cadastro de novos itens;
- a atualização de qualquer item;
- a remoção de qualquer item;
- a consulta por qualquer campo;
- a visualização dos itens cadastrados;
- permita gravar toda a base de dados em um arquivo
-> -> Faça com que seu programa carregue na árvore todos os jogos de Atari 2600 que constam na página
https://en.wikipedia.org/wiki/List_of_Atari_2600_games
*/


//------------------------
// estrutura de cada jogo
//-----------------------

typedef struct Jogo
{
    //foram escolhidos esses numeros por serem base de 2, e tamanho padrão de memoria
    char nome[TAM];
    char developer[TAM];
    char ano[TAM];
    char genero[TAM];
    char path_img_capa[TAM];
    char path_img_tela[TAM];
    struct Jogo *esq;
    struct Jogo *dir;
} Jogo;

//-----------------------
// protótipo das funções
//-----------------------

Jogo *criarJogo(const char *nome, const char *developer, const char *ano, const char *genero, const char *path_img_capa, const char *path_img_tela);
Jogo *inserirJogo(Jogo *raiz, Jogo *jogo);
Jogo *buscarJogo(Jogo *raiz, const char *nome);
Jogo *encontrarMinimo(Jogo *raiz);
Jogo *removerJogo(Jogo *raiz, const char *nome);
void imprimirJogo(Jogo *jogo);
void exibirJogos(Jogo *raiz);
void liberarArvore(Jogo *raiz);
void gravarDados(Jogo *raiz, FILE *arquivo);


//------------
// execução   
//------------

int main()
{
    Jogo *arvoreJogos = NULL;

//-------------------------------------------------------------------------
//============ CARREGANDO OS JOGOS JÁ EXISTENTES NO ATARI 2600 ============
//-------------------------------------------------------------------------

    FILE *arquivo = fopen("lista_jogos.txt", "r"); //arquivo com a lista dos jogos das tabelas do link
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

        Jogo *novoJogo = criarJogo(nome, developer, ano, genero, path_img_capa, path_img_tela); //criando com as informações da tabela

        arvoreJogos = inserirJogo(arvoreJogos, novoJogo); //inserindo na arvore
    }
    fclose(arquivo);

    int opcao;//para o switch case


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
        printf("(0) Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
//-------------------------------------------------------------------------
        case 1://CADASTRO DO NOVO JOGO
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
        case 2://ATUALIZAR JOGO JÁ EXISTENTE / EDITAR
//-------------------------------------------------------------------------

            printf("\n=====================| EDITOR |=====================\n");
            printf("Digite o nome do jogo a ser atualizado: ");
            scanf("%s", nome);
            Jogo *jogo = buscarJogo(arvoreJogos, nome);
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
        case 3: //REMOVER JOGO JÁ EXISTENTE
//-------------------------------------------------------------------------

            printf("\n====================| EXCLUIR JOGO |====================\n");
            printf("Digite o nome do jogo a ser removido: ");
            scanf("%s", nome);
            Jogo *jogoRemover = buscarJogo(arvoreJogos, nome);
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
        case 4://BUSCAR OS JOGOS POR NOME
//-------------------------------------------------------------------------

            printf("\n=====================| PESQUISAR |=====================\n");
            printf("Digite o nome do jogo: ");
            scanf("%s", nome);
            Jogo *game = buscarJogo(arvoreJogos, nome);
            if (game != NULL)
            {
                imprimirJogo(game);


                // abre a imagem da capa do jogo selecionado
                char str_arg[TAM];
                strcpy(str_arg, "explorer ");
                strcat(str_arg, game->path_img_capa);
                system(str_arg);

                // abre a imagem da tela do jogo selecionado
                strcpy(str_arg, "explorer ");
                strcat(str_arg, game->path_img_tela);
                system(str_arg);
                
                //jogos que já coloquei a imagem de teste: 
                //combat, 32-in-1, pitfall, river-raid, asterix, zaxxon e mario-bros
            }
            else
            {
                printf("Jogo nao encontrado!\n");
            }
            printf("\n========================================================\n");
            break;

//-------------------------------------------------------------------------
        case 5: //MOSTRAR TODOS OS JOGOS
//-------------------------------------------------------------------------
            exibirJogos(arvoreJogos);
            break;

 //-------------------------------------------------------------------------           
        case 6://GRAVAR BASE DE DADOS NUM ARQUIVO
//-------------------------------------------------------------------------

            printf("\n=====================| SALVAR ARQUIVO |=====================\n");
            
            printf("Digite o nome do arquivo para gravar os dados\n(acrescente '.txt' apos o nome esolhido): ");//pra ser arquivo de texto
            char nomeArquivo[TAM];
            scanf("%s", nomeArquivo);//usuario escolhe o nome

            FILE *arquivo = fopen(nomeArquivo, "w"); //abre o arquivo para escrita
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
            break;

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
    return novoJogo;
}


//-------------------------
//inserir jogo na árvore 
//--------------------------
Jogo *inserirJogo(Jogo *raiz, Jogo *jogo)
{
    if (raiz == NULL)
        return jogo;

    if (strcmp(jogo->nome, raiz->nome) < 0)
        raiz->esq = inserirJogo(raiz->esq, jogo);
    else if (strcmp(jogo->nome, raiz->nome) > 0)
        raiz->dir = inserirJogo(raiz->dir, jogo);

    return raiz;
}


//---------------------------------------
// buscar um jogo na arvore por nome
//------------------------------------------
Jogo *buscarJogo(Jogo *raiz, const char *nome)
{
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        return buscarJogo(raiz->esq, nome);
    else
        return buscarJogo(raiz->dir, nome);
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

        fprintf(arquivo,"\n====================| %s |=====================\n", raiz->nome);
        fprintf(arquivo,"Nome: %s\n", raiz->nome);
        fprintf(arquivo,"Desenvolvido por: %s\n", raiz->developer);
        fprintf(arquivo,"Ano de Lancamento: %s\n", raiz->ano);
        fprintf(arquivo,"Genero: %s\n", raiz->genero);
        fprintf(arquivo,"Capa do Jogo: %s\n", raiz->path_img_capa);
        fprintf(arquivo,"Tela do Jogo: %s\n", raiz->path_img_tela);
        fprintf(arquivo,"\n========================================================\n");

        gravarDados(raiz->dir, arquivo);
    }
}