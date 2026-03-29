#include <stdio.h>
#include <stdlib.h>

int main() {
  int tipoconversao = 0, opcao = 0;
  float valor = 0;
    printf("-------CONVERSOR DE UNIDADES-------\nInforme a conversão a ser utilizada: \n1 - Temperatura\n2 - Moeda\n3 - Distância\n");
    scanf("%d", &opcao);
    
    switch (opcao){
      case 1:
          printf("1 - Fahrenheit para Celsius\n2 - Celsius para Fahrenheit\n");
          scanf("%d", &tipoconversao);
          if (tipoconversao == 0) {
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = ((valor - 32) * 5) / 9;
            printf("O valor em celsius %.2f é em fahrenheit %.2f", valor, valor1);
          }
          else {
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = (valor * 1.8) + 32;
            printf("O valor em fahrenheit %.2f é em celsius %.2f", valor, valor1);
          }
          break;
      case 2:
          printf("1 - Dolar US$\n2- Real R$\n");
          if (tipoconversao == 1) {
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = valor * 5.23;
            printf("O valor em dolar U$ %.2f é em real R$ %.2f", valor, valor1);
          }
          else {
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = valor / 5.23;
            printf("O valor em real R$ %.2f é em dolar U$ %.2f", valor, valor1);
          }
          break;
      case 3:
          printf("1 - KM\n2- Milhas\n");
          if (tipoconversao == 1) {
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = valor * 1.852;
            printf("O valor em KM %.2f é em milhas %.2f", valor, valor1);
          }
          else{
            printf("informe o valor para conversão:\n");
            scanf("%f", &valor);
            float valor1 = 0;
            valor1 = valor / 1.852;
            printf("O valor em milhas %.2f é em KM %.2f", valor, valor1);
          }
          break;
      default:
          printf("opção invalida!\n");
    }
    
    return 0;
}