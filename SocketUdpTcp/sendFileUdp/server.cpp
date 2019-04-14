#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../classi/SocketUDP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"
#define IP_LOOP "127.0.0.1"
#define PORT 6500
#define BUFSIZE 4096

int main(int argc , char* argv[]){
    if (argc != 2) {
      printf("Usage: %s \nPath File \n", argv[0]);
      exit(1);
  }

  char * path = argv[1];
  Address me(IP_DHCP,PORT);
  SocketUDP socket(me);


  void* retur = socket.riceviFile(path);


      printf("Messaggio ricvuto  %d\n",(int*)retur);


	return 0;
}
