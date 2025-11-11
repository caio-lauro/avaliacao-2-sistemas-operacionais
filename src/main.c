#include "printcolorido.h"
#include "variaveis.h"
#include "utils.h"

int main() {
    // Ponteiro para matriz de caracteres
    matriz_t matriz;
    inicializar_matriz(&matriz, TAMANHO);

    // Ponteiro para vetor de threads
    pthread_arr_t threads;
    inicializar_vetor(&threads, TAMANHO);

    // TODO: Lógica

    liberar_matriz(&matriz);
    liberar_vetor(&threads);

    return 0;
}