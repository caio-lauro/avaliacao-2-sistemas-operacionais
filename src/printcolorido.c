#include "printcolorido.h"

void mudar_cor_de_fundo(int red, int green, int blue) {
    printf("\033[48;2;%d;%d;%dm", red, green, blue);
}

void mudar_cor_texto(int red, int green, int blue) {
    printf("\033[38;2;%d;%d;%dm", red, green, blue);
}

void resetar_cores() {
    printf("\033[0m");
}

void print_colorido_rgb(const char *mensagem, const int *RGB_FUNDO, const int *RGB_TEXTO) {
    mudar_cor_de_fundo(RGB_FUNDO[0], RGB_FUNDO[1], RGB_FUNDO[2]);
    mudar_cor_texto(RGB_TEXTO[0], RGB_TEXTO[1], RGB_TEXTO[2]);
    printf("%s", mensagem);
    resetar_cores();
}
