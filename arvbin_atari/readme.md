# Atari 2600 Game Manager
Este trabalho é propriedade de Manoela Resende, perfil [manoelargc](https://github.com/manoelargc), e foi desenvolvido como parte das avaliações da disciplina de Estrutura de Dados II do 3º semestre de Ciência da Computação - 23.1.


## Descrição do Código
Este é um código em C que implementa uma estrutura de dados de árvore binária de busca balanceada (AVL) para um gerenciador de jogos do console Atari 2600. O programa permite cadastrar, editar, excluir e pesquisar jogos, além de oferecer recursos adicionais como a gravação da base de dados em arquivo e a geração de um arquivo Graphviz.
O código implementa uma estrutura de dados de árvore binária de busca balanceada (AVL) para armazenar informações sobre jogos. 

Além disso, o código lê inicialmente um arquivo chamado "lista_jogos.txt" que contém uma lista de jogos no formato:

<nome> <developer> <ano> <genero> <path_img_capa> <path_img_tela>

A base de dados do console Atari 2600 disponível na página [List of Atari 2600 games](https://en.wikipedia.org/wiki/List_of_Atari_2600_games).


## Execução
O código contém uma função main que implementa uma interface de menu para interagir com as funcionalidades disponíveis. O usuário pode escolher uma opção digitando o número correspondente no menu.

Algumas opções disponíveis no menu são:

(1) Cadastrar novo jogo: Permite cadastrar um novo jogo informando nome, desenvolvedor, ano de lançamento, gênero e caminhos das imagens de capa e tela do jogo;
(2) Editar jogo: Permite atualizar as informações de um jogo existente;
(3) Excluir jogo: Remove um jogo da biblioteca;
(4) Consultar jogo por nome: Permite buscar um jogo pelo nome e exibe suas informações;
(5) Biblioteca de jogos: Exibe todos os jogos presentes na árvore;
(6) Gravar base de dados em arquivo: Grava os dados da árvore em um arquivo .txt;
(7) Gerar arquivo Graphviz: Gera um arquivo Graphviz (tanto .dot como .png) para visualização da árvore.

-----------
É imprescíncivel para a execução do arquivo certificar que realizou download do arquivo [lista_jogos.txt](https://github.com/manoelargc/ED2/blob/main/arvbin_atari/output/lista_jogos.txt) e que esteja na mesma pasta do código.


![Exemplo](https://github.com/manoelargc/ED2/blob/main/arvbin_atari/output/grafo_jogos.png)
