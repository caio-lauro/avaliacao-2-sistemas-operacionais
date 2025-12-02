#include "threads.h"

size_t id_mensagem = 0;
pthread_mutex_t matriz_lock, id_lock, vetor_lock;
message_queue_t *mensagens_central;

// Inicializar mutex
void inicializar_mutex() {
    if (pthread_mutex_init(&matriz_lock, NULL) != 0) {
        printf("Erro na inicialização do mutex da matriz\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&id_lock, NULL) != 0) {
        printf("Erro na inicialização do mutex do ID\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&vetor_lock, NULL) != 0) {
        printf("Erro na inicialização do mutex do vetor\n");
        exit(EXIT_FAILURE);
    }
}

void destruir_mutex() { 
    pthread_mutex_destroy(&matriz_lock);
    pthread_mutex_destroy(&id_lock);
    pthread_mutex_destroy(&vetor_lock);
}

// Função dos sensores
void *sensor(void *arg) {
    sensor_args_t *args = (sensor_args_t*)arg;

    matriz_t *matriz = args->matriz;
    const int tamanho_matriz = matriz->size;

    pthread_arr_t *arr = args->arr;
    const int threads_por_linha = tamanho_matriz / 3;
    
    int i = args->i;
    arr_element_t sensor = arr->elementos[i];

    bool fogos_vistos[3][3] = {
        {false, false, false},
        {false, false, false},
        {false, false, false}
    };

    while (true) {
        sleep(1);

        pthread_mutex_lock(&matriz_lock);

        // Verificação no 3x3 (exceto na própria thread)
        for (int i = -1; i < 2; i++) {
            int y = sensor.y + i;
            for (int j = -1; j < 2; j++) {
                int x = sensor.x + j;

                if (i == 0 && j == 0) continue;

                if (!fogos_vistos[i + 1][j + 1] && matriz->elementos[y][x].fogo) {
                    // Marcar fogo como visto
                    fogos_vistos[i + 1][j + 1] = true;

                    // ID da mensagem
                    pthread_mutex_lock(&id_lock);
                    size_t id_atual = id_mensagem++;
                    pthread_mutex_unlock(&id_lock);

                    // Criação da mensagem
                    message_t msg = message_create(id_atual, sensor.pid, x, y);

                    // Colocar mensagem na fila de mensagens da thread
                    message_queue_push(sensor.message_queue, msg);
                } else if (fogos_vistos[i + 1][j + 1] && !matriz->elementos[y][x].fogo) {
                    // Desmarcar fogo como visto
                    fogos_vistos[i + 1][j + 1] = false;
                }
            }
        }

        pthread_mutex_unlock(&matriz_lock);

        // Passar mensagens adiante
        while (!message_queue_empty(sensor.message_queue)) {
            message_t msg = message_queue_pop(sensor.message_queue);

            // Mandar mensagem para central
            if (sensor.x == 1 || sensor.y == 1 || sensor.x == tamanho_matriz - 2 || sensor.y == tamanho_matriz - 2) {
                message_queue_push(mensagens_central, msg);
            } 
            // Adicionar mensagem na fila de mensagens das threads adjacentes
            else if (msg.propagation_count > 0) {
                pthread_mutex_lock(&vetor_lock);
                msg.propagation_count--;

                // Esquerda e direita
                if (i - 1 >= 0)
                    message_queue_push(arr->elementos[i-1].message_queue, msg);
                if (i + 1 < (int)arr->size)
                    message_queue_push(arr->elementos[i+1].message_queue, msg);

                // Cima e baixo
                if (i >= threads_por_linha)
                    message_queue_push(arr->elementos[i-threads_por_linha].message_queue, msg);
                if (i + threads_por_linha < (int)arr->size)
                    message_queue_push(arr->elementos[i+threads_por_linha].message_queue, msg);

                pthread_mutex_unlock(&vetor_lock);
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

        pthread_mutex_lock(&matriz_lock);

        // Caso em que fogo atinge a thread
        if (i % 3 == 1 && j % 3 == 1) {
            int k = ((i - 1) / 3) * (matriz->size / 3) + (j - 1) / 3;

            // Fechar a thread
            pthread_mutex_lock(&vetor_lock);
            pthread_cancel(arr->elementos[k].thread_id);
            pthread_mutex_unlock(&vetor_lock);
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

    sleep(2);

    // Apagar fogo
    matriz->elementos[i][j].fogo = false;

    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

// Função da central
void *central(void *arg) {
    central_args_t *args = (central_args_t*)arg;
    matriz_t *matriz = args->matriz;
    mensagens_central = args->message_queue;

    bool *seen_messages = calloc(8, sizeof(bool));
    size_t seen_length = 8;

    while (true) {
        // Esperar por uma mensagem recebida e removê-la da fila
        message_t msg = message_queue_pop(mensagens_central);

        pthread_mutex_lock(&matriz_lock);

        if (msg.id_mensagem >= seen_length) {
            // Criação de novo vetor
            size_t new_size = seen_length * 2;
            bool *buf = realloc(seen_messages, new_size);

            // Inicializar novos valores como false (realloc não inicializa com 0)
            for (size_t i = seen_length; i < new_size; i++) buf[i] = false;

            // Atualização das informações
            seen_messages = buf;
            seen_length = new_size;
        }

        // Se a mensagem já foi vista, desbloqueia matriz_lock e prossegue para próxima mensagem
        if (seen_messages[msg.id_mensagem]) {
            pthread_mutex_unlock(&matriz_lock);
            continue;
        }

        seen_messages[msg.id_mensagem] = true;

        FILE *logs = fopen("./incendios.log", "a");

        // Separar tempo
        int hour = msg.time->tm_hour;
        int min = msg.time->tm_min;
        int sec = msg.time->tm_sec;

        // Anotar no log
        fprintf(
            logs, 
            "Incêndio detectado por %d, nas coordendas (%ld, %ld) em %02d:%02d:%02d\n", 
            msg.pid, msg.x, msg.y, hour, min, sec
        );

        // Argumentos para chamada ao bombeiro
        bombeiro_args_t args_bombeiro;
        args_bombeiro.matriz = matriz;
        args_bombeiro.i = msg.y;
        args_bombeiro.j = msg.x;

        pthread_mutex_unlock(&matriz_lock);

        // Chamada ao bombeiro (192)
        pthread_t thread_bombeiro;
        pthread_create(&thread_bombeiro, NULL, bombeiro, (void*)&args_bombeiro);

        fclose(logs);
    }

    pthread_exit(NULL);
}