#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/SocketUDP.hpp"
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
    Address addr;
    Address sendTo_(ip, portosend);

    SocketUDP socket(mySelf);
    char* buff = socket.ricevi(addr);
    bool ret = socket.invia(sendTo_, buff);
    free(buff);

    return 0;
}
