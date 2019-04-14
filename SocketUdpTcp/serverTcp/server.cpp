#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/SocketTCP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"

#define BUFSIZE 4096
#define IP_DHCP "0.0.0.0"

int main(int argc, char** argv)
{
	if(argc!=2){
		printf("Usage:%s ., PORT",argv[0]);
		exit(1);
	}
	int port = atoi(argv[1]);
    Address mySelf(IP_DHCP, port);
    Address client;
    int ret;

    SocketTCP socket(mySelf);

    socket.listenServ(5);
    int client_sock = socket.acceptConnection(&client);

    char* buff = (char*)socket.riceviServer(client_sock);

    printf("From: %s\n", client.getIp());
    printf("server ha ricevuto : %s\n", buff);

    free(buff);
	close(socket.sockkid());
    printf("\x6D"
           "a\144e\x20"
           "b\171 \x48"
           "a\162p\x72"
           "e\145t\x20"
           "S\151n\x67"
           "h\012");
    return 0;
}
