#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- ESTRUTURA ---
typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

// --- PROTÓTIPOS ---
void cadastrarProduto(Produto produtos[], int *totalProdutos);
void removerProduto(Produto produtos[], int *totalProdutos);
void listarProdutos(Produto produtos[], int totalProdutos);
void realizarVenda(Produto produtos[], int totalProdutos);
void alterarPrecoProduto(Produto produtos[], int totalProdutos);
void exibirVendasRealizadas();
void exibirBalancoGeral(); // Função atualizada
void salvarDados(Produto produtos[], int total);
int carregarDados(Produto produtos[]);
void limparBuffer();

int main() {
    Produto estoque[100];
    int totalProdutos = carregarDados(estoque);
    int opcao;
    char novoDia;

    // Pergunta inicial para resetar o dia
    printf("Deseja iniciar um NOVO DIA de vendas? (S/N): ");
    scanf(" %c", &novoDia);
    limparBuffer();

    if (toupper(novoDia) == 'S') {
        FILE *f = fopen("recibos.txt", "w");
        if (f) fclose(f);
        printf("\n>> Sistema zerado para o novo dia!\n");
    }

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

        if (scanf("%d", &opcao) != 1) { limparBuffer(); opcao = -1; continue; }
        limparBuffer();

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
    } while(opcao != 0);

    return 0;
}

// --- FUNÇÃO: BALANÇO DETALHADO (A que você pediu) ---
void exibirBalancoGeral() {
    FILE *f = fopen("recibos.txt", "r");
    char linha[150];
    int qTotalDia = 0;
    float vTotalDia = 0.0;

    if (f == NULL) {
        printf("\n[!] Nenhuma venda registrada hoje.\n");
        return;
    }

    printf("\n========== RELATORIO DE ITENS VENDIDOS ==========\n");
    printf("%-20s | %-5s | %-10s\n", "PRODUTO", "QTD", "SUBTOTAL");
    printf("-------------------------------------------------\n");

    // Lemos o arquivo linha por linha
    while (fgets(linha, sizeof(linha), f)) {
        char nomeProd[50];
        int q;
        float s;

        // sscanf tenta encontrar o padrão de um item vendido
        // %[^\t|] lê o nome do produto até encontrar o caractere '|'
        if (sscanf(linha, "Item: %[^|] | Qtd: %d | Sub: R$ %f", nomeProd, &q, &s) == 3) {
            printf("%-20s | %-5d | R$ %-10.2f\n", nomeProd, q, s);
            qTotalDia += q;
            vTotalDia += s;
        }
    }

    printf("-------------------------------------------------\n");
    printf("TOTAL DE ITENS VENDIDOS NO DIA: %d\n", qTotalDia);
    printf("VALOR TOTAL ACUMULADO NO DIA:   R$ %.2f\n", vTotalDia);
    printf("=================================================\n");

    fclose(f);
}

// --- RESTANTE DAS FUNÇÕES (PADRÃO) ---

void realizarVenda(Produto produtos[], int totalProdutos) {
    int cod, qtd, i, vendeu = 0;
    float totalCarrinho = 0;
    FILE *f = fopen("recibos.txt", "a");

    while (1) {
        printf("\nID do produto (0 p/ fechar): ");
        scanf("%d", &cod);
        limparBuffer();
        if (cod == 0) break;

        for (i = 0; i < totalProdutos; i++) {
            if (produtos[i].codigo == cod) {
                printf("Quant. para [%s]: ", produtos[i].nome);
                scanf("%d", &qtd);
                limparBuffer();
                if (qtd > 0 && qtd <= produtos[i].estoque) {
                    produtos[i].estoque -= qtd;
                    float sub = qtd * produtos[i].preco;
                    totalCarrinho += sub;
                    vendeu = 1;
                    if (f) fprintf(f, "Item: %-15s | Qtd: %d | Sub: R$ %.2f\n", produtos[i].nome, qtd, sub);
                    printf(">> Adicionado!\n");
                } else { printf("Estoque insuficiente!\n"); }
                break;
            }
        }
    }
    if (vendeu && f) fprintf(f, "FECHAMENTO VENDA: R$ %.2f\n--------------------\n", totalCarrinho);
    if (f) fclose(f);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    if (*totalProdutos >= 100) return;
    printf("ID: "); scanf("%d", &produtos[*totalProdutos].codigo); limparBuffer();
    printf("Nome: "); scanf(" %[^\n]s", produtos[*totalProdutos].nome); limparBuffer();
    printf("Preco: "); scanf("%f", &produtos[*totalProdutos].preco); limparBuffer();
    printf("Estoque: "); scanf("%d", &produtos[*totalProdutos].estoque); limparBuffer();
    (*totalProdutos)++;
}

void removerProduto(Produto produtos[], int *totalProdutos) {
    int cod, i, j;
    printf("ID p/ remover: "); scanf("%d", &cod); limparBuffer();
    for (i = 0; i < *totalProdutos; i++) {
        if (produtos[i].codigo == cod) {
            for (j = i; j < *totalProdutos - 1; j++) produtos[j] = produtos[j + 1];
            (*totalProdutos)--;
            printf("Removido!\n"); return;
        }
    }
}

void listarProdutos(Produto produtos[], int totalProdutos) {
    printf("\nID\tNOME\t\tPRECO\tESTOQUE\n");
    for (int i = 0; i < totalProdutos; i++)
        printf("%d\t%-15s\t%.2f\t%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque);
}

void alterarPrecoProduto(Produto produtos[], int totalProdutos) {
    int cod, i; float n;
    printf("ID: "); scanf("%d", &cod); limparBuffer();
    for (i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == cod) {
            printf("Novo preco: "); scanf("%f", &n); limparBuffer();
            produtos[i].preco = n; printf("Feito!\n"); return;
        }
    }
}

void exibirVendasRealizadas() {
    FILE *f = fopen("recibos.txt", "r");
    char c;
    if (!f) return;
    while ((c = fgetc(f)) != EOF) printf("%c", c);
    fclose(f);
}

void salvarDados(Produto produtos[], int total) {
    FILE *f = fopen("estoque.dat", "wb");
    if (f) { fwrite(produtos, sizeof(Produto), total, f); fclose(f); }
}

int carregarDados(Produto produtos[]) {
    FILE *f = fopen("estoque.dat", "rb");
    if (!f) return 0;
    int i = 0;
    while (i < 100 && fread(&produtos[i], sizeof(Produto), 1, f) == 1) i++;
    fclose(f);
    return i;
}
