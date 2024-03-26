#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {

    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
        abort();

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(31415);
    inet_aton("127.0.0.0", (struct in_addr *)&my_addr.sin_addr.s_addr);

    if (connect(cfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        abort();

    while (1) {
        int count;
        scanf("%d", &count);
        if(count < 1 || count > 10) count = 10;

        printf("3.");
        for(int i = 0; i < count; i++) {
            char digit = '0' + i;
            int bytes_sent = send(cfd, &digit, 1, 0);
            char response[3];
            int bytes_read = recv(cfd, response,  sizeof(response), 0);
            if (bytes_read < sizeof(response)) return 0;
            printf("%c", response[2]);
        }
        printf("\r\n");
    }
}