#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "variaveis.h"
#include "printcolorido.h"

void inicializar_matriz(matriz_t *matriz, int tamanho);
void inicializar_vetor(pthread_arr_t *arr, int tamanho);
void imprimir_matriz(matriz_t *matriz);
void liberar_matriz(matriz_t *matriz);
void liberar_vetor(pthread_arr_t *arr);
