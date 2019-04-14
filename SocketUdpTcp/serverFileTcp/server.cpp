#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/SocketTCP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"
#define PORT 6500
#define BUFSIZE 4096

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s \nPath File \n", argv[0]);
        exit(1);
    }
    Address mySelf(IP_DHCP, PORT);
    Address client;
    int ret;
    char* path = argv[1];

    SocketTCP socket(mySelf);

    socket.listenServ(5);
    int client_sock = socket.acceptConnection(&client);
    void* retur = socket.riceviFileServer(client_sock, path);

    printf("File ricevuto, lunghezza file |%d|byte\n", (int*)retur);
    printf("\x6D"
           "a\144e\x20"
           "b\171 \x48"
           "a\162p\x72"
           "e\145t\x20"
           "S\151n\x67"
           "h\012");

    return 0;
}
