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
#define PORT 8001
#define BUFSIZE 4096
#define IP_DHCP "0.0.0.0"

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: %s \nServer IP ,Server PORT\n", argv[0]);
        exit(1);
    }
    int portosend;
    portosend = atoi(argv[2]);
    char* ip;
    int serverport;
    ip = argv[1];
    Address mySelf(IP_DHCP, PORT);
    Address sentTo_(ip, portosend);
    struct sockaddr_in sentTou = sentTo_.getSockaddr_in();
    struct sockaddr_in addrMyself = mySelf.getSockaddr_in();
    struct sockaddr_in mitt;
    int sockid;
    int serverlen;
    char buf[BUFSIZE + 1];
    int clientlen = (socklen_t)sizeof(struct sockaddr_in);
    int ret;
    char* hostaddrp;
    struct hostent* hostinfo; /* struttura per avere informazioni sul client host  */

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

    /*La funzione bind() assegna l'indirizzo rappresentato nella struttura
	 sockaddr al socket cui si riferisce il descrittore socketId.
	 socketId -> descrittore del socket (endpoint) creato precedentemente.
	 addrMySelf -> struttura sockaddr_in castata in sockaddr * per rispettare
	               la parametrizzazione della funzione e rappresentante
	               indirizzo e porta di comunicazione.
	 sizeof(struct sockaddr_in) -> lunghezza della struttura puntata dal 
	                               parametro precedente.*/
    if (bind(sockid, (struct sockaddr*)&addrMyself,
            sizeof(addrMyself))
        < 0)
        error("ERRORE non sono riuscito in binding");

    /*
     * La funzione recvfrom() riceve un messaggio da un socket. 
     */
    bzero(buf, BUFSIZE + 1);
    ret = recvfrom(sockid, buf, BUFSIZE, 0,
        (struct sockaddr*)&mitt, (socklen_t*)&clientlen);
    if (ret < 0)
        error("ERRORE in recvfrom");

    buf[ret] = '\0';

    /*Address client(mitt);
    
    char* clientToStr=client.toString();
   
    printf("From: %s\n", clientToStr);
    printf("server ha ricevuto [%d/%d] bytes: %s\n", strlen(buf), ret, buf);
    free(clientToStr);
*/

    /* 
     * sendto
     */
    /*char msg[] = "I'm your server";*/
    ret = sendto(sockid, buf, strlen(buf), 0,
        (struct sockaddr*)&sentTou, (socklen_t)sizeof(struct sockaddr_in));
    if (ret < 0) {
        error("ERROR in sendto");
    }
    else {
        printf("Messaggio Inviato :)\n");
    };

    close(sockid);
    return 0;
}
