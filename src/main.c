#include "../include/printcolorido.h"
#include "../include/variaveis.h"

int main() {
    const char matriz[TAMANHO][TAMANHO];

    int fundo[3] = RGB_FUNDO;
    int texto[3] = {100, 0, 200};
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (i >= 1 && j >= 1) {

            }
            print_colorido_rgb(FOGO, fundo, texto);
        }
        printf("\n");
    }

    return 0;
}
