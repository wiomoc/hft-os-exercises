#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<unistd.h>

void* thread_main(void* arg) {
    int n = (int)((uintptr_t)arg);
    int seconds_to_sleep = (rand() % 10) + 1;
    printf("Thread %d is going to sleep for %d s\r\n", n, seconds_to_sleep);
    sleep(seconds_to_sleep);
    printf("Thread %d finished sleeping\r\n", n);
    return NULL;
}

#define N 10
#define M 5
int main(int argc, char** argv) {
    pthread_t threads[N];
    for(int n = 0; n < N; n++) {
        pthread_create(&threads[n], NULL, thread_main, (void*)((uintptr_t)n));
    }

    for(int n = 0; n < N; n++){
        pthread_join(threads[n], NULL);
    }

}