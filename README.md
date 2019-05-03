# Server-Client-Game
This is a client and a server, to implement a simple online game. The server process and the client process will run on two different machines and the communication between the two processes is achieved using Sockets.
gcc server.c -o server
./server (define port #)

gcc client.c -o client
./client (ip address of server) (port #server)

enter ip addr show to obtain the ip address on the server side
