#include <fcntl.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    if(argc < 4) {
        printf("invalid arguments expected: %s cmd1 [arg1*] | cmd2 [arg2*]\r\n", argv[0]);
        return 1;
    }

    int pipe_loc;
    for (pipe_loc = 1; pipe_loc < argc; pipe_loc++) 
        if(!strcmp(argv[pipe_loc], "|"))
            break;
    if (pipe_loc > argc - 1) {
        printf("invalid arguments expected: %s cmd1 [arg1*] | cmd2 [arg2*]\r\n", argv[0]);
    }

    char* first_pgm = argv[1];
    char* second_pgm = argv[pipe_loc + 1];

}