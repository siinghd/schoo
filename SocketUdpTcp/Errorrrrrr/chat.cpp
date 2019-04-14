#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../classi/SocketTCP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"
#define IP_LOOP "127.0.0.1"
#define PORT 6666
#define BUFSIZE 4096
#define CMD_EXIT "exit"
#define PROMPT "$>"
#define CMD_NOT_FOUND "Invalid command\n"

/*void* recevi(void* arg);
int main (int argc, char* argv[]){

  if(argc!=3){
    printf("USAGE: %s\n BroadCast [ip] , BreadCast [Port]\n",argv[0] );
    exit(1);
  }

  char* ip = argv[1];
  int port = atoi(argv[2]);
  Address mySelf(IP_DHCP,port);
  SocketTCP sock(mySelf);
  Address bc(ip,port);
	char* cmd;
  pthread_t ptid;
	pthread_create(&ptid,NULL,&ricevi,&sock);
	sock.setBroadCast(true);
	printf(PROMPT);
  sock.connectToserver(&bc);
	while( cmpStr(cmd = inputStr(),CMD_EXIT) ){
		sock.inviaClient(cmd);
		free(cmd);
	}
	if(cmd != NULL)free(cmd);
	return 0 ;
}

void* ricevi(void* arg){
	SocketTCP sock = (SocketTCP *) arg;
	Address client;
  sock.listenServ(10);
  int client_sock = sock.acceptConnection(&client);
  while(true){
		char* rec = (char *)sock.riceviServer(&client_sock);
		printf("From: %s\n", clientToStr);
		printf("Da server ho ricevuto : %s\n",  rec);
		free(clientToStr);
		free(rec);
	}
}*/