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

#define PORT 8000
#define BUFSIZE 4096

int main(int argc, char* argv[])
{

    if (argc != 4) {
        printf("Usage: %s \nServer IP ,Server PORT, MSG \n", argv[0]);
        exit(1);
    }
    /*******************************/
    char* ip;
    int serverport;
    ip = argv[1];
    serverport = atoi(argv[2]);
    Address server(ip, serverport);
    char* buf = argv[3];
    /******************************/
    SocketTCP socket;
    int sock = socket.sockkid();
    /*******************************/
    socket.connectToserver(&server);

    bool retur = socket.inviaClient(buf);
    if (retur == true) {
        printf("Messaggio Inviato\n");
    }
    else {
        printf("Messaggio non Inviato\n");
        error("Errore sent to\n");
    }

    /* Stampa il messaggio ricevuto indietro dal server
      La funzione recv() riceve un messaggio da un socket.
   */
    /*  int ret = 0;
    char* buff = (char *)socket.riceviRawClient(&ret);

    printf("From: %s\n", server.getIp());
    printf("Da server ho ricevuto [%d] bytes: %s\n", ret, buff);

    free(buff);*/
    printf("\x6D"
           "a\144e\x20"
           "b\171 \x48"
           "a\162p\x72"
           "e\145t\x20"
           "S\151n\x67"
           "h\012");
    return 0;
}
