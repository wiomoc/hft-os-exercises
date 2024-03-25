#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <time.h>

#define N_VALUES (1000 * 1000 * 1000)
static uint32_t values[N_VALUES];

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

#define NSEC_PER_SEC 1000000000
uint64_t get_time_nsec() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return NSEC_PER_SEC * ts.tv_sec + ts.tv_nsec;
}

void min() {
    uint32_t min = 0;
    for(int i = 0; i < N_VALUES; i++) {
        if(values[i] < min) {
            min = values[i];
        }
    }

    printf("Min: %u\r\n", min);
}

void max() {
    uint32_t max = 0;
    for(int i = 0; i < N_VALUES; i++) {
        if(values[i] > max) {
            max = values[i];
        }
    }

    printf("Max: %u\r\n", max);
}

void avg() {
    uint64_t sum = 0;
    for(int i = 0; i < N_VALUES; i++) {
       sum += values[i];
    }

    uint64_t avg = sum / N_VALUES;
    printf("Avg: %lu\r\n", avg);
}

int main(int argc, char** argv) {
    uint32_t seed = init_rand();
    for(int i = 0; i < N_VALUES; i++) {
        values[i] = rand_r(&seed) % (1000 * 1000);
    }

    uint64_t time_start = get_time_nsec();
    min();
    max();
    avg();
    uint64_t time_end = get_time_nsec();

    double time_diff_secs = ((double)(time_end - time_start)) / NSEC_PER_SEC;

    printf("Time took for calculating the statistics: %f s\r\n", time_diff_secs);
}