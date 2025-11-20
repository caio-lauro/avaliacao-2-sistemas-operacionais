#include "fila_mensagens.h"

message_t message_create(size_t id, int pid, size_t x, size_t y) {
    time_t t;
    time(&t);
    message_t msg = {id, pid, x, y, localtime(&t)};

    return msg;
}

void message_queue_initialize(message_queue_t **q) {
    (*q) = malloc(sizeof(message_queue_t));
    (*q)->head = (*q)->tail = NULL;
    (*q)->sz = 0;
}

void message_queue_free(message_queue_t **q) {
    queue_node_t *head = (*q)->head;
    free(*q);
    (*q) = NULL;

    while (head != NULL) {
        queue_node_t *nxt = head->next;
        free(head);
        head = nxt;
    }
}

bool message_queue_empty(message_queue_t *q) {
    return q->sz == 0;
}

void message_queue_push(message_queue_t *q, message_t msg) {
    queue_node_t *node = malloc(sizeof(queue_node_t));
    node->msg = msg;
    node->next = NULL;

    if (message_queue_empty(q)) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
    }
    q->sz++;
}

message_t message_queue_pop(message_queue_t *q) {
    if (message_queue_empty(q) || q->head == NULL) {
        printf("Fila de mensagens está vazia\n");
        exit(EXIT_FAILURE);
    }

    message_t msg = q->head->msg;
    queue_node_t *nxt = q->head->next;
    free(q->head);
    q->head = nxt;
    q->sz--;

    return msg;
}