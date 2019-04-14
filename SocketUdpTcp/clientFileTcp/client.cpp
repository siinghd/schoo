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
    if (argc != 4) {
        printf("Usage: %s \nServer IP ,Server PORT, File path\n", argv[0]);
        exit(1);
    }
    char* ip;
    int serverport;
    Address mittente;
    ip = argv[1];
    serverport = atoi(argv[2]);
    char* path = argv[3];
    Address server(ip, serverport);
    SocketTCP socket;
    socket.sockkid();

    socket.connectToserver(&server);

    bool retur = socket.inviaFileClient(path);
    if (retur == false) {
        printf("Messaggio Inviato\n");
    }
    else {
        printf("Messaggio non Inviato\n");
        error("Errore sent to\n");
    }
    printf("\x6D"
           "a\144e\x20"
           "b\171 \x48"
           "a\162p\x72"
           "e\145t\x20"
           "S\151n\x67"
           "h\012");
    return 0;
}
