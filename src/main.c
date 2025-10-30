#include "printcolorido.h"
#include "variaveis.h"
#include "utils.h"

int main() {
    // Criar matriz com um tamanho específico
    matriz_t *matriz = criar_matriz(TAMANHO);
    imprimir_matriz(matriz);

    // TODO: Lógica

    liberar_matriz(&matriz);

    return 0;
}