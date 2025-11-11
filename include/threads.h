#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "variaveis.h"

typedef struct {
    matriz_t *matriz;
    pthread_arr_t *arr;
} incendiaria_args_t;

typedef struct {
    matriz_t *matriz;
    int i, j;
} bombeiro_args_t;

typedef struct {
    matriz_t *matriz;
    arr_element_t sensor;
} sensor_args_t;

typedef struct {
    matriz_t *matriz;
    // Fila de mensagens (global)
} central_args_t;

void *sensor(void *arg);
void *incendiaria(void *arg);
void *bombeiro(void *arg);
void *central(void *arg);