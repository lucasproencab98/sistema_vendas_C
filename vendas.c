#include <stdio.h>   //Biblioteca para entrada e saída//
#include <stdlib.h>  //Biblioteca padrão// 
#include <string.h>  //Manipulação de strings//
#include <ctype.h>   //Funções como toupper (letras maiúsculas)//

//Struct que representa um produto do sistema//
typedef struct {
    nt codigo;        //Código identificador do produto//
    char nome[50];     //Nome do produto//
    float preco;       //Preço do produto//
    int estoque;       //Quantidade disponível no estoque//
} Produto;

//Declaração das funções usadas no sistema//
void cadastrarProduto(Produto produtos[], int *totalProdutos);
void removerProduto(Produto produtos[], int *totalProdutos);
void listarProdutos(Produto produtos[], int totalProdutos);
void realizarVenda(Produto produtos[], int totalProdutos);
void alterarPrecoProduto(Produto produtos[], int totalProdutos);
void limparBuffer();

int main() {
    Produto estoque[100]; //Vetor que armazena até 100 produtos//
    int totalProdutos = 0; //Começa vazio//
    int opcao;

    do {
        printf("\n===== SISTEMA DE VENDAS =====\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Listar Estoque\n");
        printf("4. Realizar Venda\n");
        printf("5. Alterar Preco\n");
        printf("0. Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
            continue;
        }
        limparBuffer();

        switch(opcao) {
            case 1: cadastrarProduto(estoque, &totalProdutos); break;
            case 2: removerProduto(estoque, &totalProdutos); break;
            case 3: listarProdutos(estoque, totalProdutos); break;
            case 4: realizarVenda(estoque, totalProdutos); break;
            case 5: alterarPrecoProduto(estoque, totalProdutos); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}

//Função Realizar venda//
void realizarVenda(Produto produtos[], int totalProdutos) {
    int cod, qtd, i;
    float total = 0;

    while (1) {
        printf("\nID do produto (0 para sair): ");
        scanf("%d", &cod);
        limparBuffer();

        if (cod == 0) break;

        for (i = 0; i < totalProdutos; i++) {
            if (produtos[i].codigo == cod) {

                printf("Quantidade: ");
                scanf("%d", &qtd);
                limparBuffer();

                if (qtd > 0 && qtd <= produtos[i].estoque) {
                    produtos[i].estoque -= qtd;
                    float sub = qtd * produtos[i].preco;
                    total += sub;

                    printf("Item vendido! Subtotal: R$ %.2f\n", sub);
                } else {
                    printf("Estoque insuficiente!\n");
                }
                break;
            }
        }
    }

    printf("Total da venda: R$ %.2f\n", total);
}

//FUnção Limpa buffer//
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Função Cadastrar produto//
void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    if (*totalProdutos >= 100) return;

    printf("ID: "); scanf("%d", &produtos[*totalProdutos].codigo); limparBuffer();
    printf("Nome: "); scanf(" %[^\n]", produtos[*totalProdutos].nome); limparBuffer();
    printf("Preco: "); scanf("%f", &produtos[*totalProdutos].preco); limparBuffer();
    printf("Estoque: "); scanf("%d", &produtos[*totalProdutos].estoque); limparBuffer();

    (*totalProdutos)++;
}

//FUnção Remover produto//
void removerProduto(Produto produtos[], int *totalProdutos) {
    int cod, i, j;

    printf("ID: "); scanf("%d", &cod); limparBuffer();

    for (i = 0; i < *totalProdutos; i++) {
        if (produtos[i].codigo == cod) {

            for (j = i; j < *totalProdutos - 1; j++)
                produtos[j] = produtos[j + 1];

            (*totalProdutos)--;
            printf("Removido!\n");
            return;
        }
    }

    printf("Produto nao encontrado!\n");
}

//Função Listar produtos//
void listarProdutos(Produto produtos[], int totalProdutos) {
    printf("\nID\tNOME\t\tPRECO\tESTOQUE\n");

    for (int i = 0; i < totalProdutos; i++)
        printf("%d\t%-15s\t%.2f\t%d\n",
               produtos[i].codigo,
               produtos[i].nome,
               produtos[i].preco,
               produtos[i].estoque);
}

//Função Alterar preço//
void alterarPrecoProduto(Produto produtos[], int totalProdutos) {
    int cod;
    float novoPreco;

    printf("ID: "); scanf("%d", &cod); limparBuffer();

    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == cod) {

            printf("Novo preco: ");
            scanf("%f", &novoPreco);
            limparBuffer();

            produtos[i].preco = novoPreco;
            printf("Preco atualizado!\n");
            return;
        }
    }

    printf("Produto nao encontrado!\n");
}
