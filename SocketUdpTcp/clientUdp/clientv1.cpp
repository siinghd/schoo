#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/Address.hpp"
#include "../lib/mylib.h"

#define IP_LOOP "127.0.0.1"
#define PORT 8000
#define BUFSIZE 4096

int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("Usage: %s \nServer IP ,Server PORT\n", argv[0]);
        exit(1);
    }

    char* ip;
    int serverport;
    ip = argv[1];
    serverport = atoi(argv[2]);

    Address server(ip, serverport);
    struct sockaddr_in addrServer = server.getSockaddr_in();
    struct sockaddr_in mitt;
    int sockid;
    int serverlen;
    char buf[BUFSIZE + 1];
    int ret;

    /* 
    La funzione socket() crea un endpoint di  comunicazione (socket)
	 e ritorna un descrittore di file che si riferisce 
	 all'endpoint appena creato.
	 AF_INET -> comunicazione attraverso il protocollo IPv4.
	 SOCK_DGRAM -> comunicazione non orientata alla connessione e
	              inaffidabile.
	 0 -> protocollo standard in base alle informazioni inserite 
	     nei parametri precedenti (UDP).
   */
    sockid = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockid < 0)
        error("ERRORE non sono riuscito ad aprire il socket");

    /* Far inserire il messaggio dall'utente */
    bzero(buf, BUFSIZE);
    printf("Inserire il messaggio: ");
    fgets(buf, BUFSIZE, stdin);

    /* Invia il messaggio al server 
     La funzione sendto() viene utilizzata per inviare un messaggio ad un altro socket.
     La funzione sendto() ritorna il numero di byte inviati in caso di successo.
   */
    serverlen = sizeof(addrServer);
    ret = sendto(sockid, buf, strlen(buf), 0, (struct sockaddr*)&addrServer, serverlen);
    if (ret < 0)
        error("ERRORE in sendto");

    /* Stampa il messaggio ricevuto indietro dal server
      La funzione recv() riceve un messaggio da un socket. 
   */
    ret = recvfrom(sockid, buf, strlen(buf), 0, (struct sockaddr*)&addrServer, (socklen_t*)&serverlen);
    buf[ret] = '\0';
    if (ret < 0)
        error("ERRORE in recvfrom");
    printf("Echo da server: %s\n", buf);

    close(sockid);
    return 0;
}
