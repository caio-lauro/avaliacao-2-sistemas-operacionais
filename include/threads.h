#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "variaveis.h"
#include "fila_mensagens.h"

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
    pthread_arr_t *arr;
    int i;
} sensor_args_t;

typedef struct {
    matriz_t *matriz;
    message_queue_t *message_queue;
} central_args_t;

void inicializar_mutex();
void destruir_mutex();

void *sensor(void *arg);
void *incendiaria(void *arg);
void *bombeiro(void *arg);
void *central(void *arg);