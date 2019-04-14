#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../classi/SocketUDP.hpp"
#include "../classi/Address.hpp"
#include "../lib/mylib.h"
#define IP_LOOP "127.0.0.1"
#define PORT 6666
#define BUFSIZE 4096
#define CMD_EXIT "exit"
#define PROMPT "$>"
#define CMD_NOT_FOUND "Invalid command\n"
#define BC_IP "192.168.6.255"
#define BC_PORT 5000

void* ricevi(void* arg);
Address mySelf(IP_DHCP,BC_PORT);
SocketUDP sock(mySelf);
Address bc(BC_IP,BC_PORT);

int main (int argc, char *argv[]){
	char* cmd;
pthread_t ptid;
	pthread_create(&ptid,NULL,&ricevi,NULL);
	sock.setBroadCast(true);
	printf(PROMPT);
	while( cmpStr(cmd = inputStr(),CMD_EXIT) ){
		sock.invia(bc,cmd);
		free(cmd);
	}
	if(cmd != NULL)free(cmd);
	return 0 ;
}

void* ricevi(void* arg){
	Address mit;
	while(true){
		char* rec = (char *)sock.ricevi(&mit);
		char* clientToStr = mit.toString();
		printf("From: %s\n", clientToStr);
		printf("Da server ho ricevuto : %s\n",  rec);
		free(clientToStr);
		free(rec);
	}
}
