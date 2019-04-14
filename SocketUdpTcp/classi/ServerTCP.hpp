#ifndef __SERVERTCP_HPP
#define __SERVERTCP_HPP

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

class ServerTCP {
    private:
        int sockid;
    public:
        ServerTCP();
        ServerTCP(Address myself);
        ~ServerTCP();
        int acceptConn( Address *client);
        void listenServ(int connections);
        
};
ServerTCP::ServerTCP(){};
ServerTCP::ServerTCP(Address myself){
    this->sockid = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in myself_addr = myself.getSockaddr_in();
    
    if (bind(this->sockid , (struct sockaddr*)&myself_addr,
            sizeof(struct sockaddr_in))
        < 0) {
        error("ERRORE non sono riuscito in binding\n");
    }
}
void ServerTCP::listenServ(int connections){
  if (listen(this->sockid,connections)) {
    error("Errore in listen di connessioni");
  }
}
int ServerTCP::acceptConn(Address *client){
  //struct sockaddr_in client;
  struct sockaddr_in clien;
  int c = sizeof(struct sockaddr_in);
  int client_Sock = accept(this->sockid,(struct sockaddr *)&clien,(socklen_t*)&c);
  
  if(client_Sock<0){
    error("errore in accept\n");
  }
 
  client->setSockaddr_in(clien);

  return client_Sock;
}

ServerTCP::~ServerTCP()
{
    close(this->sockid);
}



#endif //__ServerTCP_HPP