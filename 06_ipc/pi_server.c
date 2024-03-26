#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <unistd.h>

#define PI_DECIMALS "1415926535"
#define SOCK_PATH "/tmp/pi_sock"

void cleanup(int id) {
    printf("cleanup\r\n");
    unlink(SOCK_PATH);
    exit(0);
}

int main(int argc, char **argv) {
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        abort();

    struct sockaddr_un my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, SOCK_PATH,
            sizeof(my_addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *)&my_addr,
             sizeof(my_addr)) == -1)
        abort();

    if (listen(sfd, 10) == -1)
        abort();

    struct sockaddr_un peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);

    signal(SIGINT, cleanup);

    while (1) {
        int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size);
        while(1) {
            char requested_digit;
            int bytes_read = recv(cfd, &requested_digit, 1, 0);
            if(bytes_read == 0) break;
            if (requested_digit > '9' || requested_digit < '0') continue;
            int requested_digit_int = requested_digit - '0';

            char response[3];
            response[0] = requested_digit;
            response[1] = ':';
            response[2] = PI_DECIMALS[requested_digit_int];
            send(cfd, response, sizeof(response), 0);
        }
    }
}