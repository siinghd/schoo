#ifndef __CLIENTTCP_HPP
#define __CLIENTTCP_HPP

#define BUFSIZE 4096

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Address.hpp"
#include "../lib/mylib.h"

class ClientTCP {
    private: int sockid;
    public: ClientTCP();
            ~ClientTCP();
            bool connectToServer(Address *serv);
            bool inviaClient(const char* msg);
            void* riceviClient();
            bool inviaRawClient(const void* msg, int numByte);
            void* riceviRawClient( int* nBytes);
            bool inviaFileClient(char* path);
            void* riceviFileClient(char* path);
};
ClientTCP::ClientTCP(){
    this->sockid = socket(AF_INET, SOCK_STREAM, 0);
}
ClientTCP::~ClientTCP(){
    close(this->sockid);
}
bool ClientTCP::connectToServer(Address *serv){
    //struct socka
    struct sockaddr_in server = serv->getSockaddr_in();   
    if(connect(sockid,(struct sockaddr *)&server,sizeof(server))<0){
        return true;
    }
    return false;
}

bool ClientTCP::inviaClient(const char* msg){
    return inviaRawClient(msg,strlen(msg));    
}

bool ClientTCP::inviaRawClient(const void* msg, int numByte){
    if(send(sockid,msg,numByte,0)<0){
        return true;
    }
    return false;   
}
void* ClientTCP::riceviClient(){
    int size;
    char* raw_buff = (char*)riceviRawClient(&size);
    char* ret = (char*) malloc(sizeof(char) * (size+1));
    memcpy(ret,raw_buff,size);
    ret[size] = '\0';
    return ret;
}
void* ClientTCP::riceviRawClient( int* nBytes){
    char buff[BUFSIZE + 1];
    bzero(buff, BUFSIZE + 1);
    int ret = recv(sockid, buff, BUFSIZE, 0);
    if (ret < 0){
        error("ERRORE in recvfrom\n");
    }
    *nBytes = ret;
    return strdup(buff);
}
bool ClientTCP::inviaFileClient(char* path)
{
	int i=0;
  char ch;
  printf("File You have selected |%s|\n",path);
  int nPacchetti=0;
  FILE* fp = fopen(path,"r");
  if(fp == NULL){
		int n_packet = 0;
	  inviaRawClient(&n_packet,(int)sizeof(int));
		return true;
	}
    fseek(fp,0,SEEK_END);
  	int file_size=ftell(fp);
  	fseek(fp,0,SEEK_SET);
    nPacchetti =(BUFSIZE%file_size==0) ? file_size/BUFSIZE : (file_size/BUFSIZE) +1;
    printf("Numero di pachhetti |%d|\n",nPacchetti);
    inviaRawClient(&nPacchetti,(int)sizeof(int));
    int sizePacc, temp=file_size;
    for(int i= 0; i<nPacchetti; i++){
     char file_buffer[BUFSIZE];
     sizePacc= ( (temp%BUFSIZE) == 0) ? BUFSIZE : (file_size%BUFSIZE);
     printf("Lunghezza pacchetto numero|%d| is |%d|\n",i+1,sizePacc);
     fread(file_buffer,sizePacc,1,fp);
      /*while (fgets(file_buffer, sizePacc, fp) != NULL){
        printf("%s", file_buffer);
      }*/
      temp-=sizePacc;
      inviaRawClient(file_buffer,sizePacc);

  	}
  fclose(fp);
	return false;

}
/******************************/
void* ClientTCP::riceviFileClient(char* path){
	FILE * fp = fopen(path,"w");
	int size;
	int size_tot=0;
  Address mit;
	int* n_packet = (int*)riceviRawClient(&size);

  for(int i= 0; i< *n_packet; i++){
	    char* buffer = (char*) riceviRawClient(&size);
      printf("%s\n",buffer);
      printf("%d\n",size);
      size_tot+= size;
      printf("%d\n",size_tot);
		  fwrite(buffer,size,1,fp);
	}
	fclose(fp);
	return (void*)size_tot;

}

#endif //__CLIENTTCP_HPP