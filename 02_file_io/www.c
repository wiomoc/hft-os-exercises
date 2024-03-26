#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char** argv) {
    
    printf("before resolve domain name\r\n");
    struct addrinfo* res;
    if(getaddrinfo("info.cern.ch", "http", NULL, &res) != 0){
        printf("could not resolve domain name.\r\n");
        exit(1);
    }

    printf("after resolve domain name\r\n");


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
        freeaddrinfo(res);
        printf("could not connect.\r\n");
        exit(1);
    }
    freeaddrinfo(res);

    const char* get_http_request = "GET / HTTP/1.1\r\nHost: info.cern.ch\r\n\r\n";
    send(sock, get_http_request, strlen(get_http_request), 0);

    printf("start of response \r\n");
    char response_buffer [400];
    while(1) {
        int bytes_read = recv(sock, response_buffer, sizeof(response_buffer), 0);
        if(bytes_read > 0) {
            write(STDOUT_FILENO, response_buffer, bytes_read);
        } else {
            break;
        }
    }
    printf("end of response \r\n");
    
}