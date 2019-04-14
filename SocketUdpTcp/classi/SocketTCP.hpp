#ifndef __SOCKETTCP_HPP
#define __SOCKETTCP_HPP

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

class SocketTCP {
private:
    int sockid;

public:
    SocketTCP();
    SocketTCP(Address mySelf);
    int sockkid();
    /*****************************************************/
    void listenServ(int connections);
    int acceptConnection(Address *client);
    bool inviaServer(int clientSockid,const char* msg);
    void* riceviServer(int clientSockid);
    bool inviaRawServer(int clientSockid, const void* msg, int numByte);
    void* riceviRawServer(int clientSockid, int* nBytes);
    /*****************************************************/
    bool inviaClient(const char* msg);
    void* riceviClient();
    bool inviaRawClient(const void* msg, int numByte);
    void* riceviRawClient( int* nBytes);
    void connectToserver(Address *server);
    /*****************************************************/
    bool setBroadCast(bool set);
    bool inviaFileServer(char* path , int clientSockid);
    void* riceviFileServer(int clientSockid,char* path);

    bool inviaFileClient(char* path);
    void* riceviFileClient(char* path);


    void nSocketTCP();
    ~SocketTCP();
};
/******************************/
int SocketTCP::sockkid()
{
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid < 0)
        error("ERRORE non sono riuscito ad aprire il socket");
    puts("Socket created\n");
    return sockid;
}

/******************************/

SocketTCP::SocketTCP()
{
    /*sockid = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockid < 0)
    error("ERRORE non sono riuscito ad aprire il socket");*/
}
/******************************/
SocketTCP::SocketTCP(Address mySelf)
{
    int socketid = sockkid();
    //printf("%d\t %d\n", socketid,sockid);
    struct sockaddr_in myself_addr = mySelf.getSockaddr_in();
    if (bind(socketid , (struct sockaddr*)&myself_addr,
            sizeof(struct sockaddr_in))
        < 0) {
        error("ERRORE non sono riuscito in binding");
    }else{
      puts("bind done\n");
    }
}
/******************************/
void SocketTCP::listenServ(int connections){
  if (listen(sockid,connections)) {
    error("Errore in listen di connessioni");
  }else{
    puts("listening succefully\n");
  }
}
/******************************/
int SocketTCP::acceptConnection(Address *client){
  //struct sockaddr_in client;
  struct sockaddr_in clien;
  int c = sizeof(struct sockaddr_in);
  int client_Sock = accept(sockid,(struct sockaddr *)&clien,(socklen_t*)&c);
  if(client_Sock<0){
    error("errore in accept\n");
  }else{
    puts("Connesione Accettata");
  }
  return client_Sock;
}
/******************************/
void SocketTCP::connectToserver(Address *server){
  //struct sockaddr_in client;

  struct sockaddr_in serv = server->getSockaddr_in();
  if(connect(sockid,(struct sockaddr *)&serv,sizeof(serv))<0){
    error("errore in connect\n");
  }else{
    puts("Connessione Riuscita");
  }
  server->setSockaddr_in(serv);
}
/******************************/
bool SocketTCP::inviaServer(int clientSockid,const char* msg)
{
   	return inviaRawServer(clientSockid,msg,strlen(msg));
}

/******************************/
void* SocketTCP::riceviServer(int clientSockid)
{
  int size;
  char* raw_buff = (char*)riceviRawServer(clientSockid,&size);
  char* ret = (char*) malloc(sizeof(char) * (size+1));
  memcpy(ret,raw_buff,size);
  ret[size] = '\0';
  return ret;
}
/******************************/
bool SocketTCP::inviaRawServer(int clientSockid, const void* msg, int numByte)
{

    int ret = write(clientSockid, msg, numByte);
    if (ret < 0) {
        return false;
    }
    else {
        return true;
    };
}
/******************************/
void* SocketTCP::riceviRawServer(int clientSockid, int* nBytes)
{
      char buff[BUFSIZE + 1];
	    bzero(buff, BUFSIZE + 1);
	    int ret = recv(clientSockid, buff, BUFSIZE, 0);
	    if (ret < 0){
        error("ERRORE in recvfrom\n");
      }

	    *nBytes = ret;
	    return strdup(buff);
}
/******************************/
/******************************/
bool SocketTCP::inviaClient(const char* msg)
{
   	return inviaRawClient(msg,strlen(msg));
}

/******************************/
void* SocketTCP::riceviClient()
{
  int size;
  char* raw_buff = (char*)riceviRawClient(&size);
  char* ret = (char*) malloc(sizeof(char) * (size+1));
  memcpy(ret,raw_buff,size);
  ret[size] = '\0';
  return ret;
}
/******************************/
bool SocketTCP::inviaRawClient(const void* msg, int numByte)
{

    int ret = send(sockid, msg, numByte,0);
    if (ret < 0) {
        return false;
    }
    else {
        return true;
    };
}
/******************************/
void* SocketTCP::riceviRawClient(int* nBytes)
{
      char buff[BUFSIZE + 1];
	    bzero(buff, BUFSIZE + 1);
	    int ret = recv(sockid, buff, BUFSIZE, 0);
	    if (ret < 0){
			error("ERRORE in recvfrom\n");
		}
	    *nBytes = ret;
	    return strdup(buff);
}
/*****************************/
bool SocketTCP::inviaFileServer(char* path , int clientSockid)
{
	int i=0;
  char ch;
  printf("File You have selected |%s|\n",path);
  int nPacchetti=0;
  FILE* fp = fopen(path,"r");
  if(fp == NULL){
		int n_packet = 0;
	  inviaRawServer(clientSockid,&n_packet,(int)sizeof(int));
		return true;
	}
    fseek(fp,0,SEEK_END);
  	int file_size=ftell(fp);
  	fseek(fp,0,SEEK_SET);
    nPacchetti =(BUFSIZE%file_size==0) ? file_size/BUFSIZE : (file_size/BUFSIZE) +1;
    printf("Numero di pachhetti |%d|\n",nPacchetti);
    inviaRawServer(clientSockid,&nPacchetti,(int)sizeof(int));
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
      inviaRawServer(clientSockid,file_buffer,sizePacc);

  	}
  fclose(fp);
	return false;

}
/******************************/
void* SocketTCP::riceviFileServer(int clientSockid,char* path){
	FILE * fp = fopen(path,"w");
	int size;
	int size_tot=0;
  Address mit;
	int* n_packet = (int*)riceviRawServer(clientSockid,&size);

  for(int i= 0; i< *n_packet; i++){
	    char* buffer = (char*) riceviRawServer(clientSockid,&size);
      printf("Lunghezza pacchetto numero|%d| is |%d| byte\n",i+1,size);
      //printf("Messaggio all'interno del pacchetto :\n%s\n",buffer);

      size_tot+= size;
		  fwrite(buffer,size,1,fp);
	}
	fclose(fp);
	return (void*)size_tot;

}
/******************************/

/******************************/
bool SocketTCP::inviaFileClient(char* path)
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
void* SocketTCP::riceviFileClient(char* path){
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
/******************************/
bool SocketTCP::setBroadCast(bool set){
	int broadcastPermission=1;
	if(set){
		if (setsockopt(sockid, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0){
       		    error( "setsockopt error");

   		}
      return false;
	}
}


/******************************
void SocketTCP::nSocketTCP(){
   int ret = close(sockid);
   if(ret){ error("Errore In chiusura");}else{printf("Socket Chiuso\n");}
}
*/
SocketTCP::~SocketTCP()
{
    close(sockid);
}
#endif //__SOCKETTCP_HPP
