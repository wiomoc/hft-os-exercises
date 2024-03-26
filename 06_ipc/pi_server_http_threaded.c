#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PI_DECIMALS "1415926535"

int read_http_request(int cfd) {
    char header[200];
    int bytes_read = recv(cfd, header, sizeof(header), 0);
    if (bytes_read == 0)
        return -1;
    if (memcmp(header, "GET /", 5) != 0)
        return -1;
    char requested_digit = header[5];
    if (requested_digit > '9' || requested_digit < '0')
        return -1;
    return requested_digit - '0';
}

void *client_handler(void *args) {
    int cfd = (int)(uintptr_t)args;
    int requested_digit_int = read_http_request(cfd);
    if (requested_digit_int > 0) {    
        char response[100];
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: 1\r\n\r\n%c", PI_DECIMALS[requested_digit_int]);
        send(cfd, response, strlen(response), 0);
    }
    close(cfd);
}

int main(int argc, char **argv) {
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

    while (1) {
        int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size);
        pthread_t thread;
        pthread_create(&thread, NULL, client_handler, (void *)(intptr_t)cfd);
    }
}