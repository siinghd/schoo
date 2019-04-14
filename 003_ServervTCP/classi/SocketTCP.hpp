#ifndef __SOCKETTCP_HPP
#define __SOCKETTCP_HPP

#define BUFSIZE 4096
#define MAX_CONNECTIONS 50
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "Address.hpp"
#include "../lib/mylib.h"
using namespace std;
/********************************************
 * ******This is Socket TCP class************
 * ******************************************/
class SocketTCP {
protected:
    int sockid;

public:
    SocketTCP();
    ~SocketTCP();
};

SocketTCP::SocketTCP()
{
    this->sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid < 0) {
        error("ERRORE non sono riuscito ad aprire il socket");
    }
}
SocketTCP::~SocketTCP()
{
    close(this->sockid);
}

/***********************************************************************************************************/
/************************************************
 * ******This is Connection TCP class************
 * **********************************************/
class Connection {
protected:
    int connection_id;

public:
    Connection(int conn);
    ~Connection();
    bool inviaServer(const char* msg);
    void* riceviServer();
    bool inviaRawServer(const void* msg, int numByte);
    void* riceviRawServer(int* nBytes);
    bool inviaFileServer(char* path);
    bool inviaFileWeb(char* path);
    void* riceviFileServer(char* path);

    int getConn() {
        return connection_id;
    }
};
Connection::Connection(int conn)
{
    this->connection_id = conn;
}
/******************************/
bool Connection::inviaServer(const char* msg)
{

    return inviaRawServer(msg, strlen(msg));
}

/******************************/
void* Connection::riceviServer()
{
    int size;

    //printf("conn_id:%d", this->connection_id);
    //fflush(stdout);

    char* raw_buff = (char*)riceviRawServer(&size);
    char* ret = (char*)malloc(sizeof(char) * (size + 1));
    memcpy(ret, raw_buff, size);
    ret[size] = '\0';
    return ret;
}
/******************************/
bool Connection::inviaRawServer(const void* msg, int numByte)
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
    //printf("SONO NELLA RECVRAW\n");
    //fflush(stdout);
    // printf("id finale -> %d\n",this->connection_id);
    //fflush(stdout);

    int ret = recv(this->connection_id, buff, BUFSIZE, 0);
    if (ret < 0) {
        error("ERRORE in recvfrom\n");
    }
    *nBytes = ret;
    return strdup(buff);
}
/******************************/
bool Connection::inviaFileServer(char* path)
{
    int i = 0;
    char ch;
    printf("File You have selected |%s|\n", path);
    fflush(stdout);
    int nPacchetti = 0;
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        int n_packet = 0;
        inviaRawServer(&n_packet, (int)sizeof(int));
        return true;
    }
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    nPacchetti = (BUFSIZE % file_size == 0) ? file_size / BUFSIZE : (file_size / BUFSIZE) + 1;
    printf("Numero di pachhetti |%d|\n", nPacchetti);
    fflush(stdout);
    inviaRawServer(&nPacchetti, (int)sizeof(int));
    int sizePacc, temp = file_size;
    for (int i = 0; i < nPacchetti; i++) {
        char file_buffer[BUFSIZE];
        sizePacc = ((temp % BUFSIZE) == 0) ? BUFSIZE : (file_size % BUFSIZE);
        printf("Lunghezza pacchetto numero|%d| is |%d|\n", i + 1, sizePacc);
        fflush(stdout);
        fread(file_buffer, sizePacc, 1, fp);
        /*while (fgets(file_buffer, sizePacc, fp) != NULL){
        printf("%s", file_buffer);
        }*/
        temp -= sizePacc;
        inviaRawServer(file_buffer, sizePacc);
    }
    fclose(fp);
    return false;
}
/******************************/
void* Connection::riceviFileServer(char* path)
{
    FILE* fp = fopen(path, "w");
    int size;
    int size_tot = 0;
    Address mit;
    int* n_packet = (int*)riceviRawServer(&size);

    for (int i = 0; i < *n_packet; i++) {
        char* buffer = (char*)riceviRawServer(&size);
        printf("Lunghezza pacchetto numero|%d| is |%d| byte\n", i + 1, size);
        fflush(stdout);
        //printf("Messaggio all'interno del pacchetto :\n%s\n",buffer);

        size_tot += size;
        fwrite(buffer, size, 1, fp);
    }
    fclose(fp);
    return (void*)size_tot;
}
/******************************/
bool Connection::inviaFileWeb(char *path) {
    int i = 0;
    char ch;
    printf("File You have selected |%s|\n", path);
    fflush(stdout);
    int nPacchetti = 0;
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        int n_packet = 0;
        inviaRawServer(&n_packet, (int)sizeof(int));
        return true;
    }
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    nPacchetti = (BUFSIZE % file_size == 0) ? file_size / BUFSIZE : (file_size / BUFSIZE) + 1;
    printf("Numero di pachhetti |%d|\n", nPacchetti);
    fflush(stdout);
    //inviaRawServer(&nPacchetti, (int)sizeof(int));
    int sizePacc, temp = file_size;
    for (int i = 0; i < nPacchetti; i++) {
        char file_buffer[BUFSIZE];
        sizePacc = ((temp % BUFSIZE) == 0) ? BUFSIZE : (file_size % BUFSIZE);
        printf("Lunghezza pacchetto numero|%d| is |%d|\n", i + 1, sizePacc);
        fflush(stdout);
        fread(file_buffer, sizePacc, 1, fp);
        /*while (fgets(file_buffer, sizePacc, fp) != NULL){
        printf("%s", file_buffer);
        }*/
        temp -= sizePacc;
        inviaRawServer(file_buffer, sizePacc);
    }
    fclose(fp);
    return false;
}
/******************************/
Connection::~Connection()
{
}


/***********************************************************************************************************/
/*******************************************************
 * ******This is Client Connection TCP class************
 * *****************************************************/
class ClientConn : public Connection {
private:
public:
    ClientConn(int);
    ~ClientConn();
};
ClientConn::ClientConn(int conn_id)
    : Connection(conn_id)
{
}
ClientConn::~ClientConn() {}
/***********************************************************************************************************/
/*******************************************************
 * ******This is Server Connection TCP class************
 * *****************************************************/
class ConnServer : public Connection {
private:
public:
    ConnServer(int);
    ~ConnServer();
};
//passa conn_id al costruttore della sopraclasse
ConnServer::ConnServer(int conn_id)
    : Connection(conn_id)
{
}
ConnServer::~ConnServer()
{
    shutdown(connection_id, SHUT_RDWR);
}
/***********************************************************************************************************/
/********************************************
 * ******This is Client TCP class************
 * ******************************************/
class ClientTCP : public SocketTCP {
private: //int sockid;
    ClientConn* conn = NULL;

public:
    ClientTCP();
    ~ClientTCP();
    bool invia(char* msg);
    char* ricevi();
    bool invia_raw(void* msg, int length);
    void* ricevi_raw(int* length);
    bool connectToServer(Address* serv);
    bool inviaFile(char* path);
    void* riceviFile(char* path);
};
ClientTCP::ClientTCP() {}
ClientTCP::~ClientTCP()
{
    if (this->conn != NULL)
        delete this->conn;
}
bool ClientTCP::connectToServer(Address* serv)
{
    //struct socka
    struct sockaddr_in server = serv->getSockaddr_in();
    if (connect(sockid, (struct sockaddr*)&server, sizeof(server)) < 0) {
        return true;
    }
    this->conn = new ClientConn(sockid);
    return false;
}
bool ClientTCP::invia(char* msg)
{
    return conn->inviaServer(msg);
}
bool ClientTCP::inviaFile(char* path)
{
    return conn->inviaFileServer(path);
}
void* ClientTCP::riceviFile(char* path)
{
    return conn->riceviFileServer(path);
}
bool ClientTCP::invia_raw(void* msg, int length)
{
    return this->conn->inviaRawServer(msg, length);
}
char* ClientTCP::ricevi()
{
    return (char*)this->conn->riceviServer();
}
void* ClientTCP::ricevi_raw(int* length)
{
    return this->conn->riceviRawServer(length);
}

/***********************************************************************************************************/
/********************************************
 * ******This is Server TCP class************
 * *****************************************/
class ServerTCP : public SocketTCP {
private:
    list<ConnServer*> connections;
    void closeConnection();

public:
    ServerTCP(int port);
    void disconnect(ConnServer*);
    bool inviaTutti(char*);
    bool inviaTuttiRaw(void*, int);
    ~ServerTCP();
    ConnServer* acceptConn(); //Address *client

    // void listenServ(int connections);
};
ServerTCP::ServerTCP(int port)
{
    Address address(IP_DHCP, port);
    struct sockaddr_in myself_addr = address.getSockaddr_in();

    puts("Binding...\n");
    if (bind(sockid, (struct sockaddr*)&myself_addr,
             sizeof(struct sockaddr_in))
            < 0) {
        error("ERRORE non sono riuscito in binding\n");
    }
    puts("Listening...\n");
    if (listen(sockid, MAX_CONNECTIONS)) {
        error("Errore in listen di connessioni\n");
    }
}
ConnServer* ServerTCP::acceptConn( //Address *client
)
{
    //struct sockaddr_in client;
    struct sockaddr_in clien;
    int c = sizeof(struct sockaddr_in);

    puts("Accepting...\n");
    int client_Sock = accept(sockid, (struct sockaddr*)&clien, (socklen_t*)&c);

    if (client_Sock < 0) {
        error("errore in accept\n");
    }
    //printf("id iniziale socket -> %d",client_Sock);
    // client->setSockaddr_in(clien);
    ConnServer* conn = new ConnServer(client_Sock);
    this->connections.push_front(conn);
    return conn;
}
void ServerTCP::disconnect(ConnServer* conn)
{
    connections.remove(conn);
    delete conn;
}
void ServerTCP::closeConnection()
{
    while (!connections.empty())
        disconnect(connections.front());
}
bool ServerTCP::inviaTutti(char* msg)
{
    return inviaTuttiRaw(msg, strlen(msg));
}
bool ServerTCP::inviaTuttiRaw(void* msg, int len)
{
    bool ret = true;
    for (list<ConnServer*>::iterator it = connections.begin(); it != connections.end() && ret; it++) {
        ConnServer* current = *it;
        ret = current->inviaRawServer(msg, len);
    }
    return ret;
}
ServerTCP::~ServerTCP()
{
    closeConnection();
}

#endif //__SOCKETTCP_HPP
