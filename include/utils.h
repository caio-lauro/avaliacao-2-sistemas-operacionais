#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "variaveis.h"
#include "printcolorido.h"

void inicializar_matriz(matriz_t *matriz, size_t tamanho);
void inicializar_vetor(pthread_arr_t *arr, size_t tamanho);
void imprimir_matriz(matriz_t *matriz);
void liberar_matriz(matriz_t *matriz);
void liberar_vetor(pthread_arr_t *arr);
