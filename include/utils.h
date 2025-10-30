#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "variaveis.h"
#include "printcolorido.h"

matriz_t *criar_matriz(size_t tamanho);
void imprimir_matriz(matriz_t *matriz);
void liberar_matriz(matriz_t **matriz);
