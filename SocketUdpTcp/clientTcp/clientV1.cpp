#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/ClientTCP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"

int main(int argc, char **argv){
    if (argc != 4 ){ 
        printf("USAGE : %S , IP ADDRESS , PORT , MESSAGE",argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *msg = argv[3];

    ClientTCP *socketClient = new ClientTCP();
    Address *server = new Address(ip,port);
    if(socketClient->connectToServer(server)){
        error("Errore in Connect()");
    }
    while(true){
   if(socketClient->inviaClient(msg)){
        error("Errore in Send()");
    }
    }
 
    
    /* Stampa il messaggio ricevuto indietro dal server
      La funzione recv() riceve un messaggio da un socket.
   */
    int ret = 0;
    char* buff = (char *)socketClient->riceviRawClient(&ret);

    printf("From: %s\n", server->getIp());
    printf("Da server ho ricevuto [%d] bytes: %s\n", ret, buff);

    delete(buff);
    delete(server);
    delete(socketClient);
    printf("\x6D"
           "a\144e\x20"
           "b\171 \x48"
           "a\162p\x72"
           "e\145t\x20"
           "S\151n\x67"
           "h\012");
    return 0;

}
