#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>

atomic_ulong counter = ATOMIC_VAR_INIT(0);
#define N 10
#define STEPS_PER_THREAD (1000 * 1000)
#define MAX N * STEPS_PER_THREAD

void* thread_main(void* arg) {
    for(uint64_t i = 0; i < STEPS_PER_THREAD; i++) {
        atomic_fetch_add(&counter, 1);
    }
}

int main(int argc, char** argv) {
    pthread_t threads[N];

    for(int n = 0; n < N; n++) {
        pthread_create(&threads[n], NULL, thread_main, (void*)((uintptr_t)n));
    }

    for(int n = 0; n < N; n++){
        pthread_join(threads[n], NULL);
    }

    printf("expected %u actual %lu\r\n", MAX, counter);
}