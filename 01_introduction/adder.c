#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("two arguments expected: % <num1> <num2>\r\n", argv[0]);
        return 1;
    }

    int a = strtol(argv[1], NULL, 10);
    int b = strtol (argv[2], NULL, 10);
    int sum = a + b;
    printf("%d\r\n", sum);
}