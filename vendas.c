#include <stdio.h>   //Biblioteca para entrada e saída//
#include <stdlib.h>  //Biblioteca padrão// 
#include <string.h>  //Manipulação de strings//
#include <ctype.h>   //Funções como toupper (letras maiúsculas)//


//Struct que representa um produto do sistema//
typedef struct {
    int codigo;        //Código identificador do produto//
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
void exibirVendasRealizadas();
void exibirBalancoGeral(); 
void salvarDados(Produto produtos[], int total);
int carregarDados(Produto produtos[]);
void limparBuffer();

int main() {
    Produto estoque[100]; //Vetor que armazena até 100 produtos//
    int totalProdutos = carregarDados(estoque); // Carrega dados salvos//
    int opcao;
    char novoDia;

    //Pergunta se deseja iniciar um novo dia (zerar vendas)//
    printf("Deseja iniciar um NOVO DIA de vendas? (S/N): ");
    scanf(" %c", &novoDia);
    limparBuffer();

    //Se escolher 'S', limpa o arquivo de recibos//
    if (toupper(novoDia) == 'S') {
        FILE *f = fopen("recibos.txt", "w");
        if (f) fclose(f);
        printf("\n>> Sistema zerado para o novo dia!\n");
    }

    //Loop principal do sistema (menu)//
    do {
        printf("\n==============================");
        printf("\n      SISTEMA DE VENDAS       ");
        printf("\n==============================");
        printf("\n1. Cadastrar Produto");
        printf("\n2. Remover Produto");
        printf("\n3. Listar Estoque");
        printf("\n4. Realizar Venda");
        printf("\n5. Alterar Preco");
        printf("\n6. Ver Todos os Recibos");
        printf("\n7. RELATORIO DE ITENS VENDIDOS");
        printf("\n0. Sair");
        printf("\n------------------------------");
        printf("\nEscolha uma opcao: ");

        //Validação básica da entrada//
        if (scanf("%d", &opcao) != 1) { 
            limparBuffer(); 
            opcao = -1; 
            continue; 
        }
        limparBuffer();

        //Switch para escolher a opção do menu//
        switch(opcao) {
            case 1: cadastrarProduto(estoque, &totalProdutos); salvarDados(estoque, totalProdutos); break;
            case 2: removerProduto(estoque, &totalProdutos); salvarDados(estoque, totalProdutos); break;
            case 3: listarProdutos(estoque, totalProdutos); break;
            case 4: realizarVenda(estoque, totalProdutos); salvarDados(estoque, totalProdutos); break;
            case 5: alterarPrecoProduto(estoque, totalProdutos); salvarDados(estoque, totalProdutos); break;
            case 6: exibirVendasRealizadas(); break;
            case 7: exibirBalancoGeral(); break;
            case 0: printf("\nSaindo...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }
    } while(opcao != 0); //Continua até escolher sair//

    return 0;
}

//RELATÓRIO DE VENDAS DO DIA//
//Lê o arquivo de recibos e mostra um resumo das vendas//
void exibirBalancoGeral() {
    FILE *f = fopen("recibos.txt", "r"); //Abre arquivo para leitura//
    char linha[150];
    int qTotalDia = 0;
    float vTotalDia = 0.0;

    if (f == NULL) {
        printf("\n[!] Nenhuma venda registrada hoje.\n");
        return;
    }

    printf("\n========== RELATORIO DE ITENS VENDIDOS ==========\n");

    //Lê linha por linha do arquivo//
    while (fgets(linha, sizeof(linha), f)) {
        char nomeProd[50];
        int q;
        float s;

        //Extrai os dados da linha usando sscanf//
        if (sscanf(linha, "Item: %[^|] | Qtd: %d | Sub: R$ %f", nomeProd, &q, &s) == 3) {
            printf("%-20s | %-5d | R$ %-10.2f\n", nomeProd, q, s);
            qTotalDia += q;
            vTotalDia += s;
        }
    }

    printf("TOTAL DE ITENS: %d\n", qTotalDia);
    printf("VALOR TOTAL: R$ %.2f\n", vTotalDia);

    fclose(f);
}

//FUNÇÃO DE VENDA//
//Permite vender produtos e registrar no arquivo//
void realizarVenda(Produto produtos[], int totalProdutos) {
    int cod, qtd, i, vendeu = 0;
    float totalCarrinho = 0;
    FILE *f = fopen("recibos.txt", "a"); // Abre para adicionar//

    while (1) {
        printf("\nID do produto (0 p/ fechar): ");
        scanf("%d", &cod);
        limparBuffer();
        if (cod == 0) break;

        //Procura o produto//
        for (i = 0; i < totalProdutos; i++) {
            if (produtos[i].codigo == cod) {

                printf("Quant. para [%s]: ", produtos[i].nome);
                scanf("%d", &qtd);
                limparBuffer();

                //Verifica estoque//
                if (qtd > 0 && qtd <= produtos[i].estoque) {
                    produtos[i].estoque -= qtd; // Atualiza estoque//
                    float sub = qtd * produtos[i].preco;
                    totalCarrinho += sub;
                    vendeu = 1;

                    //Salva no arquivo//
                    if (f) fprintf(f, "Item: %-15s | Qtd: %d | Sub: R$ %.2f\n", produtos[i].nome, qtd, sub);

                    printf(">> Adicionado!\n");
                } else { 
                    printf("Estoque insuficiente!\n"); 
                }
                break;
            }
        }
    }

    //Finaliza a venda//
    if (vendeu && f) fprintf(f, "FECHAMENTO VENDA: R$ %.2f\n", totalCarrinho);
    if (f) fclose(f);
}

//FUNÇÃO AUXILIAR//
//Limpa o buffer do teclado//
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//CADASTRAR PRODUTO//
void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    if (*totalProdutos >= 100) return;

    printf("ID: "); scanf("%d", &produtos[*totalProdutos].codigo); limparBuffer();
    printf("Nome: "); scanf(" %[^\n]s", produtos[*totalProdutos].nome); limparBuffer();
    printf("Preco: "); scanf("%f", &produtos[*totalProdutos].preco); limparBuffer();
    printf("Estoque: "); scanf("%d", &produtos[*totalProdutos].estoque); limparBuffer();

    (*totalProdutos)++;
}

//REMOVER PRODUTO//
void removerProduto(Produto produtos[], int *totalProdutos) {
    int cod, i, j;

    printf("ID p/ remover: "); scanf("%d", &cod); limparBuffer();

    for (i = 0; i < *totalProdutos; i++) {
        if (produtos[i].codigo == cod) {
            for (j = i; j < *totalProdutos - 1; j++) 
                produtos[j] = produtos[j + 1];

            (*totalProdutos)--;
            printf("Removido!\n");
            return;
        }
    }
}

//LISTAR PRODUTOS//
void listarProdutos(Produto produtos[], int totalProdutos) {
    printf("\nID\tNOME\t\tPRECO\tESTOQUE\n");

    for (int i = 0; i < totalProdutos; i++)
        printf("%d\t%-15s\t%.2f\t%d\n",
               produtos[i].codigo,
               produtos[i].nome,
               produtos[i].preco,
               produtos[i].estoque);
}

//ALTERAR PREÇO//
void alterarPrecoProduto(Produto produtos[], int totalProdutos) {
    int cod, i; 
    float n;

    printf("ID: "); scanf("%d", &cod); limparBuffer();

    for (i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == cod) {
            printf("Novo preco: "); scanf("%f", &n); limparBuffer();
            produtos[i].preco = n;
            printf("Feito!\n");
            return;
        }
    }
}

//MOSTRAR RECIBOS//
void exibirVendasRealizadas() {
    FILE *f = fopen("recibos.txt", "r");
    char c;

    if (!f) return;

    while ((c = fgetc(f)) != EOF)
        printf("%c", c);

    fclose(f);
}

//SALVAR DADOS EM ARQUIVO//
void salvarDados(Produto produtos[], int total) {
    FILE *f = fopen("estoque.dat", "wb");

    if (f) {
        fwrite(produtos, sizeof(Produto), total, f);
        fclose(f);
    }
}

//CARREGAR DADOS//
int carregarDados(Produto produtos[]) {
    FILE *f = fopen("estoque.dat", "rb");

    if (!f) return 0;

    int i = 0;

    while (i < 100 && fread(&produtos[i], sizeof(Produto), 1, f) == 1)
        i++;

    fclose(f);
    return i;
}
