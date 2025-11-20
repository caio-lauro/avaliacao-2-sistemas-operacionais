#include "fila_mensagens.h"

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
    if (message_queue_empty(q)) {
        printf("Fila de mensagens está vazia\n");
        exit(1);
    }

    message_t msg = q->head->msg;
    queue_node_t *nxt = q->head->next;
    free(q->head);
    q->head = nxt;
    q->sz--;

    return msg;
}