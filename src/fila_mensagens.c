#include "fila_mensagens.h"

message_t message_create(size_t id, int pid, size_t x, size_t y) {
    time_t t;
    time(&t);

    origin_dir dir = {X_NONE, Y_NONE};
    message_t msg = {id, pid, x, y, localtime(&t), dir};

    return msg;
}

void message_queue_initialize(message_queue_t **q) {
    (*q) = malloc(sizeof(message_queue_t));
    (*q)->head = (*q)->tail = NULL;
    (*q)->sz = 0;

    if (pthread_mutex_init(&(*q)->mtx, NULL) != 0) {
        printf("Erro na inicialização do mutex da fila.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&(*q)->cond, NULL) != 0) {
        printf("Erro na inicialização da cond da fila.\n");
        exit(EXIT_FAILURE);
    }
}

void message_queue_free(message_queue_t **q) {
    if (q == NULL || *q == NULL) return;

    pthread_mutex_lock(&(*q)->mtx);
    queue_node_t *head = (*q)->head;
    (*q)->head = (*q)->tail = NULL;
    (*q)->sz = 0;
    pthread_mutex_unlock(&(*q)->mtx);
    
    while (head != NULL) {
        queue_node_t *nxt = head->next;
        free(head);
        head = nxt;
    }

    pthread_mutex_destroy(&(*q)->mtx);
    pthread_cond_destroy(&(*q)->cond);

    free(*q);
    (*q) = NULL;
}

bool message_queue_empty(message_queue_t *q) {
    if (q == NULL) return true;

    pthread_mutex_lock(&q->mtx);
    bool empty = (q->sz == 0 || q->head == NULL);
    pthread_mutex_unlock(&q->mtx);

    return empty;
}

void message_queue_push(message_queue_t *q, message_t msg) {
    if (q == NULL) return;

    queue_node_t *node = malloc(sizeof(queue_node_t));
    node->msg = msg;
    node->next = NULL;

    pthread_mutex_lock(&q->mtx);
    if (q->sz == 0) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->sz++;

    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mtx);
}

message_t message_queue_pop(message_queue_t *q) {
    pthread_mutex_lock(&q->mtx);
    while (q->sz == 0) {
        // Esperar por novas mensagens: garante pop bem sucedido
        pthread_cond_wait(&q->cond, &q->mtx);
    }

    message_t msg = q->head->msg;
    queue_node_t *nxt = q->head->next;
    free(q->head);
    q->head = nxt;
    q->sz--;

    pthread_mutex_unlock(&q->mtx);

    return msg;
}