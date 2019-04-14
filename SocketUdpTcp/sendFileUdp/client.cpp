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
    if (argc != 4) {
      printf("Usage: %s \nServer IP ,Server PORT, File \n", argv[0]);
      exit(1);
  }

  char* ip;
  int serverport;
  Address mittente;
  ip = argv[1];
  serverport = atoi(argv[2]);
  char * path = argv[3];
  Address server(ip, serverport);
  SocketUDP socket;
  socket.sockkid();
printf("%s\n",path);
  bool retur = socket.inviaFile(path,server);
  if (retur == false) {
      printf("Messaggio Inviato\n");
  }
  else {
      printf("Messaggio non Inviato\n");
      error("Errore sent to\n");
  }

	return 0;
}
