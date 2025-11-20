#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    pthread_t id;
    size_t x, y;
    struct tm *time;
} message_t;

typedef struct queue_node_t{
    message_t msg;
    struct queue_node_t *next;
} queue_node_t;

typedef struct {
    queue_node_t *head, *tail;
    size_t sz;
} message_queue_t;

void message_queue_initialize(message_queue_t **q);
void message_queue_free(message_queue_t **q);
bool message_queue_empty(message_queue_t *q);

void message_queue_push(message_queue_t *q, message_t msg);
message_t message_queue_pop(message_queue_t *q);