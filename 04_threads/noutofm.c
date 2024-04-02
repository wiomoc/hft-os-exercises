#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<unistd.h>

void* thread_main(void* arg) {
    int m = (int)((uintptr_t)arg);
    int seconds_to_sleep = (rand() % 10) + 1;
    printf("Thread %d is going to sleep for %d s\r\n", m, seconds_to_sleep);
    sleep(seconds_to_sleep);
    printf("Thread %d finished sleeping\r\n", m);
    return NULL;
}

#define M 10
#define N 5
int main(int argc, char** argv) {
    pthread_t threads[M];
    for(int m = 0; m < M; m++) {
        pthread_create(&threads[m], NULL, thread_main, (void*)((uintptr_t)m));
    }

    for(int m = 0; m < M; m++){
        pthread_join(threads[m], NULL);
    }

}