#include <assert.h>

#include "printcolorido.h"
#include "variaveis.h"
#include "utils.h"
#include "threads.h"

int main() {
    // Tamanho da matriz
    const int tamanho = TAMANHO;

    // Garantir que o tamanho é divisível por 3
    assert(tamanho % 3 == 0);

    // Ponteiro para matriz de caracteres
    matriz_t matriz;
    inicializar_matriz(&matriz, tamanho);

    // Ponteiro para vetor de threads
    pthread_arr_t threads;
    inicializar_vetor(&threads, tamanho);

    imprimir_matriz(matriz);

    // Thread IDs
    pthread_t thread_central, thread_incendiaria;

    // Criação das threads sensores
    for (size_t i = 0; i < threads.size; i++) {
        pthread_t thread_sensor;
        sensor_args_t sensor_args;
        sensor_args.matriz = &matriz;
        sensor_args.sensor = threads.elementos[i];
        pthread_create(&thread_sensor, NULL, sensor, (void*)&sensor_args);
        threads.elementos[i].thread_id = thread_sensor;
    }

    // Criação da thread incendiária
    incendiaria_args_t incendiaria_args;
    incendiaria_args.matriz = &matriz;
    incendiaria_args.arr = &threads;
    pthread_create(&thread_incendiaria, NULL, incendiaria, (void*)&incendiaria_args);

    // Criação da thread central
    central_args_t central_args;
    central_args.matriz = &matriz;
    pthread_create(&thread_central, NULL, central, (void*)&central_args);

    // Impressão da matriz
    while (true) {
        sleep(1);
        imprimir_matriz(matriz);
    }

    pthread_exit(NULL);

    liberar_matriz(&matriz);
    liberar_vetor(&threads);

    return 0;
}