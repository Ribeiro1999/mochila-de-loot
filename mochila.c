/*
    Trabalho: Sistema de Inventário (Mochila de Loot Inicial)
    Disciplina: Estrutura de Dados

    Ideia geral do programa:
    O jogador começa a partida e precisa organizar os itens que vai
    encontrando (armas, munição, kits médicos, ferramentas...) dentro
    de uma mochila que tem espaço limitado (aqui, no máximo 10 itens).

    Para representar cada item eu uso uma struct chamada Item, e para
    guardar vários itens eu uso um vetor (lista sequencial) de Item.

    As operações pedidas são:
    - inserirItem   - cadastra um novo item na mochila
    - removerItem   - tira um item da mochila pelo nome
    - listarItens   - mostra todos os itens que estão na mochila
    - buscarItem    - procura um item pelo nome (busca sequencial)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Capacidade máxima da mochila, conforme pedido no enunciado
#define MAX_ITENS 10

/*
    Struct Item
    Cada item da mochila tem:
    - nome: identifica o item (ex: "Kit Medico")
    - tipo: categoria do item (ex: "arma", "municao", "cura")
    - quantidade: quantos daquele item o jogador está carregando
*/
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

/*
    Protótipos das funções.
    Deixei os protótipos aqui em cima só para o main ficar mais
    organizado lá embaixo e eu poder chamar as funções antes de
    "implementar" elas de fato no arquivo.
*/
void inserirItem(Item mochila[], int *totalItens);
void removerItem(Item mochila[], int *totalItens);
void listarItens(Item mochila[], int totalItens);
int buscarItem(Item mochila[], int totalItens, char nomeBuscado[]);
void limparBufferEntrada(void);

int main() {
    // vetor (lista sequencial) que representa a mochila do jogador
    Item mochila[MAX_ITENS];

    // contador de quantos itens já foram cadastrados na mochila
    int totalItens = 0;

    // variável para guardar a opção escolhida no menu
    int opcao;

    printf("=== MOCHILA DE SOBREVIENCIA - CODIGO DA ILHA ===\n");

    /*4
        Uso um "do while" aqui porque eu quero que o menu apareça
        pelo menos uma vez, e continue aparecendo até o jogador
        escolher a opção de sair (0).
    */
    do {
        printf("\n----------- MENU -----------\n");
        printf("1 - Cadastrar item na mochila\n");
        printf("2 - Remover item da mochila\n");
        printf("3 - Buscar item pelo nome\n");
        printf("4 - Listar todos os itens\n");
        printf("0 - Sair do sistema\n");
        printf("-----------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // depois de ler um número com scanf, sobra um "\n" no buffer,
        // então limpo esse buffer antes de usar fgets mais na frente
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                // listagem obrigatória após cada operação
                listarItens(mochila, totalItens);
                break;

            case 2:
                removerItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;

            case 3: {
                char nomeBuscado[30];
                printf("Digite o nome do item que deseja buscar: ");
                fgets(nomeBuscado, sizeof(nomeBuscado), stdin);
                // fgets pega o \n no final, então eu tiro ele
                nomeBuscado[strcspn(nomeBuscado, "\n")] = '\0';

                int posicao = buscarItem(mochila, totalItens, nomeBuscado);

                if (posicao != -1) {
                    printf("\nItem encontrado na posicao %d da mochila:\n", posicao + 1);
                    printf("Nome: %s\n", mochila[posicao].nome);
                    printf("Tipo: %s\n", mochila[posicao].tipo);
                    printf("Quantidade: %d\n", mochila[posicao].quantidade);
                } else {
                    printf("\nItem \"%s\" nao foi encontrado na mochila.\n", nomeBuscado);
                }

                // também listo depois da busca, como pedido no enunciado
                listarItens(mochila, totalItens);
                break;
            }

            case 4:
                listarItens(mochila, totalItens);
                break;

            case 0:
                printf("\nSaindo do sistema... boa sorte na partida!\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/*
    Função inserirItem
    Responsável por cadastrar um novo item na mochila, desde que
    ainda tenha espaço (menos de MAX_ITENS itens cadastrados).
*/
void inserirItem(Item mochila[], int *totalItens) {

    // primeiro verifico se a mochila já está cheia
    if (*totalItens >= MAX_ITENS) {
        printf("\nMochila cheia! Nao e possivel cadastrar mais itens.\n");
        return; // encerra a função aqui, nao tem o que fazer
    }

    // uso um Item temporário para ir preenchendo os dados antes
    // de colocar de fato dentro do vetor mochila
    Item novoItem;

    printf("\n--- Cadastro de novo item ---\n");

    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // remove o \n

    printf("Tipo do item (arma, municao, cura, ferramenta...): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada(); // limpa o \n que sobra depois do scanf

    // agora sim, coloco o item preenchido na próxima posição livre
    // do vetor, que é exatamente a posição "totalItens"
    mochila[*totalItens] = novoItem;

    // incremento o contador, já que agora tem mais um item guardado
    (*totalItens)++;

    printf("\nItem \"%s\" cadastrado com sucesso na mochila!\n", novoItem.nome);
}

/*
    Função removerItem
    Procura o item pelo nome informado e, se encontrar, remove ele
    do vetor "puxando" os itens seguintes uma posição para trás,
    para não deixar buraco no meio da lista.
*/
void removerItem(Item mochila[], int *totalItens) {

    // se a mochila está vazia nem faz sentido tentar remover nada
    if (*totalItens == 0) {
        printf("\nA mochila esta vazia, nao ha itens para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    // reaproveito a função de busca sequencial para achar a posição
    int posicao = buscarItem(mochila, *totalItens, nomeRemover);

    if (posicao == -1) {
        printf("\nItem \"%s\" nao encontrado na mochila.\n", nomeRemover);
        return;
    }

    /*
        Aqui está a lógica de remoção da lista sequencial:
        eu desloco todos os itens que vêm depois da posição
        removida uma casa para a esquerda, sobrescrevendo o
        item que vai sair.
    */
    for (int i = posicao; i < *totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    // diminuo o contador, pois agora tem um item a menos
    (*totalItens)--;

    printf("\nItem \"%s\" removido da mochila com sucesso!\n", nomeRemover);
}

/*
    Função listarItens
    Percorre o vetor inteiro e imprime os dados de cada item.
    Chamada depois de cada operação, conforme pedido no enunciado,
    para o jogador sempre ver o estado atual da mochila.
*/
void listarItens(Item mochila[], int totalItens) {

    printf("\n=== ITENS ATUAIS NA MOCHILA (%d/%d) ===\n", totalItens, MAX_ITENS);

    // se não tem nenhum item, já aviso e nem entro no laço
    if (totalItens == 0) {
        printf("A mochila esta vazia no momento.\n");
        return;
    }

    // percorro o vetor do início até o total de itens cadastrados
    for (int i = 0; i < totalItens; i++) {
        printf("\nItem %d:\n", i + 1);
        printf("  Nome: %s\n", mochila[i].nome);
        printf("  Tipo: %s\n", mochila[i].tipo);
        printf("  Quantidade: %d\n", mochila[i].quantidade);
    }
}

/*
    Função buscarItem (busca sequencial)
    Percorre o vetor item por item, comparando o nome de cada
    posição com o nome buscado, até achar (ou chegar ao final
    sem encontrar).

    Retorna:
    - a posição (índice) do item no vetor, se encontrar
    - -1, se não encontrar nada com esse nome
*/
int buscarItem(Item mochila[], int totalItens, char nomeBuscado[]) {

    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 quando as duas strings sao iguais
        if (strcmp(mochila[i].nome, nomeBuscado) == 0) {
            return i; // achou! devolve a posicao
        }
    }

    // se o laço terminou e não encontrou, o item não existe na mochila
    return -1;
}

/*
    Função limparBufferEntrada
    Depois de usar scanf("%d", ...), sobra um "\n" no buffer de
    entrada, e isso atrapalha o fgets que vem logo depois (ele lê
    esse \n sozinho e acha que a string terminou vazia).
    Essa função só "come" os caracteres que sobraram até achar
    o \n ou o fim do arquivo.
*/
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // nao faz nada, so descarta o caractere
    }
}