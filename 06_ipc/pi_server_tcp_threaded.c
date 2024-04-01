#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PI_DECIMALS "1415926535"

void *client_handler(void *args)
{
    int cfd = (int)(uintptr_t) args;
    while (1) {
        char requested_digit;
        int bytes_read = recv(cfd, &requested_digit, 1, 0);
        if (bytes_read == 0)
            break;
        if (requested_digit > '9' || requested_digit < '0')
            continue;
        int requested_digit_int = requested_digit - '0';

        char response[5];
        response[0] = requested_digit;
        response[1] = ':';
        response[2] = PI_DECIMALS[requested_digit_int];
        response[3] = '\r';
        response[4] = '\n';
        send(cfd, response, sizeof(response), 0);
    }
}

int main(int argc, char **argv)
{

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        abort();

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(31415);

    if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        abort();

    if (listen(sfd, 10) == -1)
        abort();

    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);

    while (1)
    {
        int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size);
        pthread_t thread;
        pthread_create(&thread, NULL, client_handler, (void*)(intptr_t) cfd);
    }
}