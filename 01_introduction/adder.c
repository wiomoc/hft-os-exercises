#include <stdio.h>
#include <stdlib.h>

int __attribute__ ((noinline)) add(int a, int b) {
    return a + b;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("two arguments expected: %s <num1> <num2>\r\n", argv[0]);
        return 1;
    }

    int a = strtol(argv[1], NULL, 10);
    int b = strtol (argv[2], NULL, 10);
    int sum = add(a, b);
    printf("%d\r\n", sum);
}