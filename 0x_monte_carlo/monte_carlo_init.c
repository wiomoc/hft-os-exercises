#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>

#define ROUNDS 800 * 1000 * 1000

static uint32_t init_rand() {
    int fd = open("/dev/urandom", 0);
    uint32_t seed = 0;
    if(read(fd, &seed, sizeof(seed)) != sizeof(seed)) {
        close(fd);
        exit(1);
    }
    close(fd);
}

float rand_double(uint32_t* seed) {
    return ((double) rand_r(seed)) / RAND_MAX;
}

int main(int argc, char**  argv) {
    uint32_t seed = init_rand();

    int n_in_circle = 0;
    for(int i = 0; i < ROUNDS; i++) {
        double x = rand_double(&seed);
        double y = rand_double(&seed);

        if ((x*x + y*y) <= 1.0) n_in_circle++;
    }

    double ratio_in_circle_not_in_circle = ((double) n_in_circle) / ((double)ROUNDS);
    printf("approx. PI: %.8lf\r\n", ratio_in_circle_not_in_circle * 4);
}