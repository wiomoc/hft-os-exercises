# Exercise 06: IPC

## Named Pipes
Open two terminals: Shell 1 and Shell 2
1. Shell 1: Create a named pipe with `mkfifo mypipe`.
2. Shell 1: Write something in this pipe using `echo Hello > mypipe`
3. Shell 2: Execute `cat mypipe`. What happens in Shell 1 and Shell 2?
3. Shell 2: Execute `cat mypipe`. What happens in Shell 2?
2. Shell 1: Write something in this pipe using `echo World > mypipe`.  What happens in Shell 1 and Shell 2?


## Sockets with PIGen
### Server
Sockets offer a more sufisticated way of implemeting IPC. Sockets are bidirektional and provide the basis auf a Server-Client Architecture, meaning one or many client are able to connect to a single listening server socket and exchange data with it.
In this tasks we want to develop a server implement the PIGen Service. The PIGen Service was specified in [RFC3091](https://www.ietf.org/rfc/rfc3091.txt), its purpose is to provide the Pi digit generation for Clients lacking PI Support. The RFC was actually intended as an April Fool's joke - it is simply more economical to store 10 digits of PI locally rather than implement a huge client. Nevertheless, the protocol offers itself to be implemented for training purposes due to its simplicity.

After a client connected to a server, the client sents an ASCII encoded single-digit e.g. `8` over the connection, the server than looks up the respective n-th digit of PI and responds in the following format `<n>:<n-th digit of pi>` e.g `8:3`. This back-and-forth is repeated until the connection is terminated. 

Initialy implement the server socket using a unix socket. You find guideiance at `man 7 unix`. Take a closer look at the eaxmple at the end. Also make youself familiar with the following methods: `socket`, `bind`, `listen` and `accept`.

For testing use `nc -u -l <name of your socket>`

### Client
Now develop a programm that serves as the client part of the protocol. The program should offer a REPL where the user should be able to interactively request the nth decimal place.

You will need to use the following functions: `socket`, `connect`, `send`, `recv`.

After you implemented and tried the programm successfuly, try to open the programm on two terminal in parallel. What are you experiancing?

### TCP Sockets
Unix Sockets only enable local IPC communication, for enabling Remote-Procedure-Call (RPC) over a network like the internet rewrite the server and the client to use tcp sockets instead.

Hint: Rewrite the instanciation of the `sockaddr` struct in the client like the following:
```c
struct sockaddr_in my_addr;
memset(&my_addr, 0, sizeof(my_addr));
my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(31415);
inet_aton("127.0.0.1", (struct in_addr *) &my_addr.sin_addr.s_addr);
```


### Multiple Sockets Parallized with Threads
When you ran the client several times in parallel, you will have noticed that only one session was possible at a time. To enable multiple simultaneous connections/sessions, use threads so that after a connection is acceptes, a new thread is started that handles the requests on this connection. You will only need to use the function `pthread_create` out of pthread.


