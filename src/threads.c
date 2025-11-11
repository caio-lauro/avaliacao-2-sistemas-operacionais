#include "threads.h"

// Função dos sensores
void *sensor(void *arg) {
    sensor_args_t *args = (sensor_args_t*)arg;
    matriz_t *matriz = args->matriz;
    const int tamanho = matriz->size;
    arr_element_t sensor = args->sensor;
    while (true) {
        sleep(1);

        for (int i = -1; i < 2; i++) {
            int y = sensor.y + i;
            for (int j = -1; j < 2; j++) {
                int x = sensor.x + j;

                if (i == 0 && j == 0) continue;

                if (matriz->elementos[y][x].fogo) {
                    // Mandar mensagem para central
                    if (sensor.x == 1 || sensor.y == 1 || sensor.x == tamanho - 2 || sensor.y == tamanho - 2) {
                        // Adicionar na fila de mensagens global
                    } else {
                        // Adicionar mensagem na fila de mensagens das threads adjacentes
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
}

// Função da incendiária
void *incendiaria(void *arg) {
    incendiaria_args_t *args = (incendiaria_args_t*)arg;
    matriz_t *matriz = args->matriz;
    pthread_arr_t *arr = args->arr;

    while (true) {
        sleep(5);
        int i = rand() % matriz->size;
        int j = rand() % matriz->size;

        // Caso em que fogo atinge a thread
        if (i % 3 == 1 && j % 3 == 1) {
            int k = ((i - 1) / 3) * (matriz->size / 3) + (j - 1) / 3;

            // Fechar a thread
            pthread_cancel(arr->elementos[k].thread_id);
        }

        matriz->elementos[i][j].fogo = true;
    }

    pthread_exit(NULL);
}

// Função do bombeiro
void *bombeiro(void *arg) {
    bombeiro_args_t *args = (bombeiro_args_t*)arg;
    matriz_t *matriz = args->matriz;
    int i = args->i;
    int j = args->j;

    matriz->elementos[i][j].fogo = false;

    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

// Função da central
void *central(void *arg) {
    central_args_t *args = (central_args_t*)arg;
    matriz_t *matriz = args->matriz;

    FILE *logs = fopen("./incendios.log", "a");

    while (true) {
        // Esperar enquanto a fila de mensagens está vazia

        // Coordenadas do incêndio da fila de mensagens
        int i = 0, j = 0;

        // Anotar no log

        // Argumentos para chamada ao bombeiro
        bombeiro_args_t args_bombeiro;
        args_bombeiro.matriz = matriz;
        args_bombeiro.i = i;
        args_bombeiro.j = j;

        // Chamada ao bombeiro (192)
        pthread_t thread_bombeiro;
        pthread_create(&thread_bombeiro, NULL, bombeiro, (void*)&args_bombeiro);
    }

    fclose(logs);

    pthread_exit(NULL);
}