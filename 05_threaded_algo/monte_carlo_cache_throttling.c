#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <pthread.h>

#define THREAD_COUNT 16
#define ROUNDS_PER_THREAD (50 * 1000 * 1000)
#define ROUNDS (THREAD_COUNT * ROUNDS_PER_THREAD)

uint32_t seeds[THREAD_COUNT];
static void init_rand() {
    int fd = open("/dev/urandom", 0);
    int seeds_bytes = sizeof(seeds);
    if(read(fd, seeds, seeds_bytes) != seeds_bytes) {
        close(fd);
        exit(1);
    }
    close(fd);
}

float rand_double(int tid) {
    return ((double) rand_r((uint32_t*)&seeds[tid])) / RAND_MAX;
}

static long n_in_circle[THREAD_COUNT];

void* monte_carlo(void* arg) {
    int tid = (int)(uintptr_t)arg;
    for(int i = 0; i < ROUNDS_PER_THREAD; i++) {
        double x = rand_double(tid);
        double y = rand_double(tid);

        if ((x*x + y*y) <= 1.0) n_in_circle[tid]++;
    }
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