all: pi_server pi_client pi_server_tcp pi_client_tcp pi_server_tcp_threaded pi_server_http_threaded

pi_server: pi_server.c
	gcc -o pi_server pi_server.c

pi_server_tcp: pi_server_tcp.c
	gcc -o pi_server_tcp pi_server_tcp.c

pi_server_tcp_threaded: pi_server_tcp_threaded.c
	gcc -o pi_server_tcp_threaded pi_server_tcp_threaded.c

pi_server_http_threaded: pi_server_http_threaded.c
	gcc -o pi_server_http_threaded pi_server_http_threaded.c

pi_client: pi_client.c
	gcc -o pi_client pi_client.c

pi_client_tcp: pi_client_tcp.c
	gcc -o pi_client_tcp pi_client_tcp.c