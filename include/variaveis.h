#pragma once

#include <stdbool.h>
#include <pthread.h>

#define FOGO "🔥 "
#define TAMANHO 30

typedef struct {
    char c;
    bool fogo;
} matriz_element_t;

typedef struct {
    matriz_element_t **elementos;
    size_t size;
} matriz_t;

typedef struct {
    int pid;
    pthread_t thread_id;
    message_queue_t *message_queue;
    int x, y;
} arr_element_t;

typedef struct {
    arr_element_t *elementos;
    size_t size;
} pthread_arr_t;

// Definir cores de fundo
extern int fundo[3];
extern int fundo_borda[3];

// Definir cores de texto
extern int texto_colunas[3];
extern int texto_linhas[3];
extern int texto[3];