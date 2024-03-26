#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <pthread.h>

#define THREAD_COUNT 8
#define ROUNDS_PER_THREAD (100 * 1000 * 1000)
#define ROUNDS (THREAD_COUNT * ROUNDS_PER_THREAD)

static uint32_t init_rand() {
    int fd = open("/dev/urandom", 0);
    uint32_t seed = 0;
    if(read(fd, &seed, sizeof(seed)) != sizeof(seed)) {
        close(fd);
        exit(1);
    }
    close(fd);
    return seed;
}

float rand_double(uint32_t* seed) {
    return ((double) rand_r(seed)) / RAND_MAX;
}

static long n_in_circle[THREAD_COUNT];


void* monte_carlo(void* arg) {
    long n_in_circle_local = 0;
    int tid = (int)(uintptr_t)arg;
    uint32_t seed = init_rand();
    printf("start: %d\r\n", tid);
    for(int i = 0; i < ROUNDS_PER_THREAD; i++) {
        double x = rand_double(&seed);
        double y = rand_double(&seed);

        if ((x*x + y*y) <= 1.0) n_in_circle_local++;
    }
    n_in_circle[tid] = n_in_circle_local;
    printf("stop: %d\r\n", tid);
}

int main(int argc, char**  argv) {
    init_rand();
    
    pthread_t threads[THREAD_COUNT];
    for(int tid = 0; tid < THREAD_COUNT; tid++) {
        n_in_circle[tid] = 0;
        pthread_create(&threads[tid], NULL, monte_carlo, (void*)(uintptr_t) tid);
    }

    int n_in_circle_total = 0;
    for(int tid = 0; tid < THREAD_COUNT; tid++) {
        pthread_join(threads[tid], NULL);
        n_in_circle_total += n_in_circle[tid];
    }

    double ratio_in_circle_not_in_circle = ((double) n_in_circle_total) / ((double)ROUNDS);
    printf("approx. PI: %.8lf\r\n", ratio_in_circle_not_in_circle * 4);
}