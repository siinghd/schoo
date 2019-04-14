#ifndef __ADDRESS_HPP
#define __ADDRESS_HPP

#define IP_DHCP "0.0.0.0"
#define PORT_ZERO 0
#define LEN_ADDRESS_STR 25

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../lib/mylib.h"

class Address {
private:
    char* ip;
    int port;
    static pthread_mutex_t mtx_set;

public:
    Address(char*, int);
    Address();
    Address(const Address&);
    Address(struct sockaddr_in);

    char* getIp();
    int getPort();
    void setIp(char*);
    void setPort(int);

    struct sockaddr_in getSockaddr_in();
    void setSockaddr_in(struct sockaddr_in);

    char* toString();

    ~Address();
};
pthread_mutex_t Address::mtx_set = PTHREAD_MUTEX_INITIALIZER;
Address::Address(char* ip, int port)
{
    this->ip = strdup(ip);
    this->port = port;
}

Address::Address()
{
    this->ip = strdup(IP_DHCP);
    this->port = PORT_ZERO;
}

Address::Address(const Address& address)
{
    this->ip = strdup(address.ip);
    this->port = address.port;
}

Address::Address(struct sockaddr_in address)
{
    this->ip = strdup(inet_ntoa(address.sin_addr));
    this->port = ntohs(address.sin_port);
}

char* Address::getIp()
{
    return strdup(this->ip);
}

int Address::getPort()
{
    return (this->port);
}

void Address::setIp(char* ip)
{

    if (ip) {
        free(this->ip);
    }
    this->ip = strdup(ip);
}

void Address::setPort(int port)
{
    this->port = port;
}

struct sockaddr_in Address::getSockaddr_in()
{

    struct sockaddr_in retAddress;

    retAddress.sin_family = AF_INET;
    retAddress.sin_port = htons(this->port);
    inet_aton(this->ip, &retAddress.sin_addr);
    for (int i = 0; i < 8; i++)
        retAddress.sin_zero[i] = 0;

    return retAddress;
}

void Address::setSockaddr_in(struct sockaddr_in address)
{

    pthread_mutex_lock(&mtx_set);
    setIp(inet_ntoa(address.sin_addr));
    this->port = ntohs(address.sin_port);
    pthread_mutex_unlock(&mtx_set);
}

char* Address::toString()
{
    char temp[LEN_ADDRESS_STR];
    sprintf(temp, "[%s:%d]", this->ip, this->port);
    return strdup(temp);
}

Address::~Address()
{
    free(this->ip);
}

#endif //__ADDRESS_HPP
