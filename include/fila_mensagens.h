#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    size_t id_mensagem;
    int pid;
    size_t x, y;
    struct tm *time;
    size_t propagation_count;
} message_t;

typedef struct queue_node_t{
    message_t msg;
    struct queue_node_t *next;
} queue_node_t;

typedef struct {
    queue_node_t *head, *tail;
    size_t sz;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} message_queue_t;

message_t message_create(size_t id, int pid, size_t x, size_t y);

void message_queue_initialize(message_queue_t **q);
void message_queue_free(message_queue_t **q);
bool message_queue_empty(message_queue_t *q);

void message_queue_push(message_queue_t *q, message_t msg);
message_t message_queue_pop(message_queue_t *q);