#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 3

char floresta[N][N];

typedef struct{
    int x, y;
    bool temFogo;
} Mensagem_t;

Mensagem_t msg_central = {-1, -1, false};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_central = PTHREAD_COND_INITIALIZER;

void printMatriz(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%c ", floresta[i][j]);
        }
        printf("\n");
    }
    printf("----------\n");
}

void *geraFogo(void *arg){
    while(true){
        sleep(5);
        pthread_mutex_lock(&mutex);
        int x = rand() % N;
        int y = rand() % N;
        floresta[x][y] = '@';
        printMatriz();
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *bombeiro(void *arg){
    int *p = (int *)arg;
    sleep(2);
    pthread_mutex_lock(&mutex);
    floresta[p[0]][p[1]] = '-';
    printMatriz();
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *sensor(void *arg){
    int x = 1, y = 1;
    while(true){
        sleep(1);
        pthread_mutex_lock(&mutex);

        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                int nx = x + i, ny = y + j;
                if(nx >= 0 && ny >= 0 && nx < N && ny < N){
                    if(floresta[nx][ny] == '@'){
                        msg_central.x = nx;
                        msg_central.y = ny;
                        msg_central.temFogo = true;
                        pthread_cond_signal(&cond_central);
                    }
                } 
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *central(void *arg){
    while(true){
        pthread_mutex_lock(&mutex);
        while(!msg_central.temFogo)
            pthread_cond_wait(&cond_central, &mutex);
        
        int x = msg_central.x;
        int y = msg_central.y;
        msg_central.temFogo = false;
        pthread_mutex_unlock(&mutex);

        int pos[2];
        pos[0] = x;
        pos[1] = y;

        pthread_t thread_bombeiro;
        pthread_create(&thread_bombeiro, NULL, bombeiro, pos);
        pthread_detach(thread_bombeiro);
    }
    return NULL;
}

int main(){
    srand(time(NULL));

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            floresta[i][j] = '-';
        }
    }
    floresta[1][1] = 'T';
    printMatriz();

    pthread_t thread_sensor, thread_central, thread_fogo;
    pthread_create(&thread_sensor, NULL, sensor, NULL);
    pthread_create(&thread_central, NULL, central, NULL);
    pthread_create(&thread_fogo, NULL, geraFogo, NULL);

    pthread_join(thread_sensor, NULL);
    pthread_join(thread_central, NULL);
    pthread_join(thread_fogo, NULL);

    return 0xCAFEEE;
}