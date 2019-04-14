#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

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
#include "ServerTCP.hpp"
#include "../lib/mylib.h"

class Connection : public ServerTCP 
{
    private:
        int connection_id;
    public:
        Connection(int conn);
        ~Connection();
        bool inviaServer(const char* msg);
        void* riceviServer();
        bool inviaRawServer( const void* msg, int numByte);
        void* riceviRawServer( int* nBytes);
        bool inviaFileServer(char* path );
        void* riceviFileServer(char* path);


};
Connection::Connection(int conn){

    this->connection_id=conn;
}
/******************************/
bool Connection::inviaServer(const char* msg)
{
   	return inviaRawServer(msg,strlen(msg));
}

/******************************/
void* Connection::riceviServer()
{
  int size;
  char* raw_buff = (char*)riceviRawServer(&size);
  char* ret = (char*) malloc(sizeof(char) * (size+1));
  memcpy(ret,raw_buff,size);
  ret[size] = '\0';
  return ret;
}
/******************************/
bool Connection::inviaRawServer( const void* msg, int numByte)
{

    int ret = write(this->connection_id, msg, numByte);
    if (ret < 0) {
        return false;
    }
    else {
        return true;
    };
}
/******************************/
void* Connection::riceviRawServer(int* nBytes)
{
      char buff[BUFSIZE + 1];
	    bzero(buff, BUFSIZE + 1);
	    int ret = recv(this->connection_id, buff, BUFSIZE, 0);
	    if (ret < 0){
        error("ERRORE in recvfrom\n");
      }

	    *nBytes = ret;
	    return strdup(buff);
}
/******************************/
bool Connection::inviaFileServer(char* path  )
{
	int i=0;
  char ch;
  printf("File You have selected |%s|\n",path);
  int nPacchetti=0;
  FILE* fp = fopen(path,"r");
  if(fp == NULL){
		int n_packet = 0;
	  inviaRawServer(&n_packet,(int)sizeof(int));
		return true;
	}
    fseek(fp,0,SEEK_END);
  	int file_size=ftell(fp);
  	fseek(fp,0,SEEK_SET);
    nPacchetti =(BUFSIZE%file_size==0) ? file_size/BUFSIZE : (file_size/BUFSIZE) +1;
    printf("Numero di pachhetti |%d|\n",nPacchetti);
    inviaRawServer(&nPacchetti,(int)sizeof(int));
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
      inviaRawServer(file_buffer,sizePacc);

  	}
  fclose(fp);
	return false;

}
/******************************/
void* Connection::riceviFileServer(char* path){
	FILE * fp = fopen(path,"w");
	int size;
	int size_tot=0;
  Address mit;
	int* n_packet = (int*)riceviRawServer(&size);

  for(int i= 0; i< *n_packet; i++){
	    char* buffer = (char*) riceviRawServer(&size);
      printf("Lunghezza pacchetto numero|%d| is |%d| byte\n",i+1,size);
      //printf("Messaggio all'interno del pacchetto :\n%s\n",buffer);

      size_tot+= size;
		  fwrite(buffer,size,1,fp);
	}
	fclose(fp);
	return (void*)size_tot;

}
/******************************/
Connection::~Connection()
{
    int ret = shutdown (connection_id,SHUT_RDWR);
    if (ret){
        error("Errore in shutdown");
    }
}



#endif //__CONNECTION_HPP