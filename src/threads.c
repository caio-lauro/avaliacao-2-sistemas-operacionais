#include "threads.h"

message_queue_t *mensagens_central;

// Função dos sensores
void *sensor(void *arg) {
    sensor_args_t *args = (sensor_args_t*)arg;
    matriz_t *matriz = args->matriz;
    const int tamanho = matriz->size;
    pthread_arr_t *arr = args->arr;
    int i = args->i;
    arr_element_t sensor = arr->elementos[i];
    while (true) {
        sleep(1);

        pthread_mutex_lock(&matriz_lock);

        // Verificação no 3x3 (exceto na própria thread)
        for (int i = -1; i < 2; i++) {
            int y = sensor.y + i;
            for (int j = -1; j < 2; j++) {
                int x = sensor.x + j;

                if (i == 0 && j == 0) continue;

                if (matriz->elementos[y][x].fogo) {
                    // Criação da mensagem
                    message_t msg;
                    msg.id = sensor.pid;
                    time_t t;
                    time(&t);
                    msg.time = localtime(&t);
                    msg.x = x;
                    msg.y = y;

                    // Mandar mensagem para central
                    if (sensor.x == 1 || sensor.y == 1 || sensor.x == tamanho - 2 || sensor.y == tamanho - 2) {
                        message_queue_push(mensagens_central, msg);
                    } 
                    // Adicionar mensagem na fila de mensagens das threads adjacentes
                    else {
                        // Esquerda e direita
                        message_queue_push(arr->elementos[i-1].message_queue, msg);
                        message_queue_push(arr->elementos[i+1].message_queue, msg);

                        // Cima e baixo
                        message_queue_push(arr->elementos[i-tamanho].message_queue, msg);
                        message_queue_push(arr->elementos[i+tamanho].message_queue, msg);
                    }
                }
            }
        }

        pthread_mutex_unlock(&matriz_lock);
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

        pthread_mutex_lock(&matriz_lock);

        // Caso em que fogo atinge a thread
        if (i % 3 == 1 && j % 3 == 1) {
            int k = ((i - 1) / 3) * (matriz->size / 3) + (j - 1) / 3;

            // Fechar a thread
            pthread_cancel(arr->elementos[k].thread_id);
        }

        matriz->elementos[i][j].fogo = true;

        pthread_mutex_unlock(&matriz_lock);
    }

    pthread_exit(NULL);
}

// Função do bombeiro
void *bombeiro(void *arg) {
    bombeiro_args_t *args = (bombeiro_args_t*)arg;
    matriz_t *matriz = args->matriz;
    int i = args->i;
    int j = args->j;

    pthread_mutex_lock(&matriz_lock);

    // Apagar fogo
    matriz->elementos[i][j].fogo = false;

    pthread_mutex_unlock(&matriz_lock);

    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

// Função da central
void *central(void *arg) {
    central_args_t *args = (central_args_t*)arg;
    matriz_t *matriz = args->matriz;
    mensagens_central = args->message_queue;

    FILE *logs = fopen("./incendios.log", "a");

    while (true) {
        // Esperar enquanto a fila de mensagens está vazia
        while (message_queue_empty(mensagens_central));

        // Mensagem recebida
        message_t msg = message_queue_pop(mensagens_central);

        // Verifica se a mensagem é nova

        // Anotar no log

        // Argumentos para chamada ao bombeiro
        bombeiro_args_t args_bombeiro;
        args_bombeiro.matriz = matriz;
        args_bombeiro.i = msg.y;
        args_bombeiro.j = msg.x;

        // Chamada ao bombeiro (192)
        pthread_t thread_bombeiro;
        pthread_create(&thread_bombeiro, NULL, bombeiro, (void*)&args_bombeiro);
    }

    fclose(logs);

    pthread_exit(NULL);
}