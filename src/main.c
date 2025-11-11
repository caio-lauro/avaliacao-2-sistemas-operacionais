#include <assert.h>

#include "printcolorido.h"
#include "variaveis.h"
#include "utils.h"

int main() {
    // Tamanho da matriz
    const int tamanho = TAMANHO;

    // Garantir que o tamanho é divisível por 3
    assert(tamanho % 3 == 0);

    // Ponteiro para matriz de caracteres
    matriz_t matriz;
    inicializar_matriz(&matriz, tamanho);

    // Ponteiro para vetor de threads
    pthread_arr_t threads;
    inicializar_vetor(&threads, tamanho);

    imprimir_matriz(matriz);

    // TODO: Lógica

    liberar_matriz(&matriz);
    liberar_vetor(&threads);

    return 0;
}