#include <fcntl.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

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

    int p[2];
    pipe(p);
    int recv_pipe = p[0];
    int send_pipe = p[1];

    pid_t child_1_pid = fork();
    if(child_1_pid) {
        //parent
        pid_t child_2_pid = fork();
        if(child_2_pid) {
            // wait for both childs to finish
            wait(NULL);
            wait(NULL);
        } else {
            //child 2
            int first_args_len = pipe_loc - 1;
            char** first_argv = malloc(sizeof(char*) * (first_args_len + 1));
            for(int i = 0; i < first_args_len; i++) first_argv[i] = argv[1 + i];
            first_argv[first_args_len] = NULL;

            dup2(send_pipe, STDOUT_FILENO);
            execvp(first_pgm, first_argv);
        }
    } else {
        //child 1
        int second_args_len = argc - (pipe_loc + 1);
        char** second_argv = malloc(sizeof(char*) * (second_args_len + 1));
        for(int i = 0; i < second_args_len; i++) second_argv[i] = argv[pipe_loc + 1 + i];
        second_argv[second_args_len] = NULL;
        
        dup2(recv_pipe, STDIN_FILENO);
        execvp(second_pgm, second_argv);
    }
}