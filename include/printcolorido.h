#pragma once

#include <stdio.h>
#include <stdlib.h>

#define BLACK   0
#define RED     1    
#define GREEN   2    
#define YELLOW  3
#define BLUE    4 
#define MAGENTA 5
#define CYAN    6    
#define WHITE   7

void mudar_cor_de_fundo(int red, int green, int blue);
void mudar_cor_texto(int red, int green, int blue);
void resetar_cores();
void print_colorido_rgb(const char *mensagem, const int *RGB_FUNDO, const int *RGB_TEXTO);
