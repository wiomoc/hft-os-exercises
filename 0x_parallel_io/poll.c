#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdbool.h>
#include <errno.h>

#define PI_DECIMALS "1415926535"

#define MAX_CLIENTS 10

struct client_state
{
    bool in_use;
    bool read_available;
    bool write_available;
    bool awaiting_read;
    int outgoing_bytes_offset;
    int n_pending_outgoing_bytes;
    char pending_outgoing_bytes[5];
};

int client_handler(int cfd, struct client_state *state)
{
    if (state->read_available && state->awaiting_read)
    {
        char requested_digit;
        int n_bytes_read = recv(cfd, &requested_digit, 1, 0);
        state->read_available = false;
        if (n_bytes_read > 0)
        {
            if (requested_digit <= '9' && requested_digit >= '0')
            {
                int requested_digit_int = requested_digit - '0';
                state->pending_outgoing_bytes[0] = requested_digit;
                state->pending_outgoing_bytes[1] = ':';
                state->pending_outgoing_bytes[2] = PI_DECIMALS[requested_digit_int];
                state->pending_outgoing_bytes[3] = '\r';
                state->pending_outgoing_bytes[4] = '\n';
                state->outgoing_bytes_offset = 0;
                state->n_pending_outgoing_bytes = 5;
                state->awaiting_read = false;
            }
        }
        else
        {
            if (read_available == 0 || !(errno == EWOULDBLOCK || errno == EAGAIN))
            {
                // EOF or error - terminate connection
                return -1;
            }
        }
    }

    if (state->write_available && state->n_pending_outgoing_bytes > 0)
    {
        int n_bytes_written = send(cfd, state->pending_outgoing_bytes + state->outgoing_bytes_offset,
                                   state->n_pending_outgoing_bytes, 0);
        state->write_available = false;
        if (n_bytes_written > 0)
        {
            state->n_pending_outgoing_bytes -= n_bytes_written;
            state->outgoing_bytes_offset += n_bytes_written;
            if (state->n_pending_outgoing_bytes == 0)
            {
                state->outgoing_bytes_offset = 0;
                state->awaiting_read = true;
            }
        }
        else
        {
            if (!(errno == EWOULDBLOCK || errno == EAGAIN))
            {
                // EOF or error - terminate connection
                return -1;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    int sfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
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

    struct pollfd pollfds[MAX_CLIENTS + 1];
    struct client_state client_states[MAX_CLIENTS];

    pollfds[MAX_CLIENTS].fd = sfd;
    pollfds[MAX_CLIENTS].events = POLLIN;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        pollfds[i].fd = -1;
        pollfds[i].events = 0;
        client_states[i].in_use = false;
    }

    while (1)
    {
        poll(pollfds, MAX_CLIENTS + 1, -1);
        if (pollfds[MAX_CLIENTS].revents & POLLIN)
        {
            int cfd = accept4(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size, SOCK_NONBLOCK);
            if (cfd > 0)
            {
                for (int i = 0; i < MAX_CLIENTS + 1; i++)
                {
                    if (!client_states[i].in_use)
                    {
                        client_states[i].in_use = true;
                        client_states[i].awaiting_read = true;
                        client_states[i].n_pending_outgoing_bytes = 0;
                        client_states[i].read_available = false;
                        client_states[i].write_available = false;
                        pollfds[i].fd = cfd;
                        break;
                    }
                }
            }
        }
        pollfds[MAX_CLIENTS].revents = 0;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if(!client_states[i].in_use) continue;
            if (pollfds[i].revents & (POLLIN | POLLOUT))
            {
                client_states[i].read_available |= ((pollfds[i].revents & POLLIN) != 0);
                client_states[i].write_available |= ((pollfds[i].revents & POLLOUT) != 0);
                int res = client_handler(pollfds[i].fd, &client_states[i]);
                if (res < 0)
                {
                    pollfds[i].fd = -1;
                    client_states[i].in_use = false;
                }
            }
            pollfds[i].events = 0;
            if (client_states[i].n_pending_outgoing_bytes > 0)
            {
                pollfds[i].events |= POLLOUT;
            }
            if (client_states[i].awaiting_read)
            {
                pollfds[i].events |= POLLIN;
            }
            pollfds[i].revents = 0;
        }
    }
}