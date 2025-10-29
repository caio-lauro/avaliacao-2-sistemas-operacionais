#include "printcolorido.h"
#include "variaveis.h"

void imprimir_matriz();

int main() {
    imprimir_matriz();

    return 0;
}

void imprimir_matriz() {
    // Definir cores de fundo
    int fundo[3] = RGB_FUNDO;
    int fundo_borda[3] = {80, 80, 80};

    // Definir cores de texto
    int texto_colunas[3] = {200, 200, 200};
    int texto_linhas[3] = {50, 50, 255};
    int texto[3] = {100, 0, 200};

    // Variável para armazenar números das guias com espaçamento
    char numero_guia[5];

    // Variável para armazenar o tamanho da matriz de impressão
    const int TAMANHO_MATRIZ_IMPRESSAO = 3 * TAMANHO + 3;

    // Variável para armazenar o tamanho da borda
    const int TAMANHO_BORDA = TAMANHO_MATRIZ_IMPRESSAO + 2;

    // Borda superior
    for (int j = 0; j < TAMANHO_BORDA; j++) {
        print_colorido_rgb(" ", fundo_borda, NULL);
    }
    printf("\n");

    // Borda lateral da esquerda
    print_colorido_rgb(" ", fundo_borda, NULL);

    // Imprimir guias de colunas
    // Espaçamento inicial
    print_colorido_rgb("  ", fundo, texto_colunas);

    // Números das colunas
    for (int j = 0; j < TAMANHO; j++) {
        sprintf(numero_guia, " %2i", j);
        print_colorido_rgb(numero_guia, fundo, texto_colunas);
    }
    print_colorido_rgb(" ", fundo, texto_colunas);

    // Borda lateral da direita
    print_colorido_rgb(" ", fundo_borda, NULL);

    printf("\n");

    // Imprimir linhas da matriz
    for (int i = 0; i < TAMANHO; i++) {
        // Borda lateral da esquerda
        print_colorido_rgb(" ", fundo_borda, NULL);

        // Imprimir guia de linhas
        sprintf(numero_guia, "%2i ", i);
        print_colorido_rgb(numero_guia, fundo, texto_linhas);

        // Imprimir elementos da linha
        for (int j = 0; j < TAMANHO; j++) {
            if (i >= 1 && j >= 1 && ((i - 1) % 3 == 0) && ((j - 1) % 3 == 0)) {
                // TODO: criar thread
                print_colorido_rgb(" T ", fundo, texto);
            } else {
                print_colorido_rgb(" - ", fundo, texto);
            }
        }

        // Borda lateral da direita
        print_colorido_rgb(" ", fundo_borda, NULL);

        printf("\n");
    }

    // Borda inferior
    for (int j = 0; j < TAMANHO_BORDA; j++) {
        print_colorido_rgb(" ", fundo_borda, NULL);
    }
    printf("\n");
}