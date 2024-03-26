#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char **argv) {

    int cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cfd == -1)
        abort();

    struct sockaddr_un my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, "/tmp/pi_sock",
            sizeof(my_addr.sun_path) - 1);

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