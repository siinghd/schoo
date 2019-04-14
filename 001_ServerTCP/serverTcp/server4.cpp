#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../classi/Address.hpp"
#include "../lib/mylib.h"
#include "../classi/SocketTCP.hpp"
#include "../classi/WebServer.hpp"
#include <execinfo.h>
#define STOP "stop"



typedef struct  InfoServer {
    ServerTCP* server;
    WebServer* webserver;
    ConnServer* serverConn;
} ServerInfo;


/* DEBUG */
void* show_params(ServerInfo* params, char* where, int n) {

    printf("===== %s =begin= %d ===\n",where, n);

    printf("params: %x\n",
           params);
    printf("->socket: %x\n",
           params->server);
    printf("->connection: %x(%d)\n",
           params->serverConn, params->serverConn->getConn());
    printf("->webserver: %x(%d)\n",
           params->webserver);
    printf("===== %s =end====\n",where);

    fflush(stdout);

}

/* HANDLER FOR SIGSEV */
void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void *manageConnection(void *serverConn);
void fineRichiesta(ServerInfo * serverinformation);
int main (int argc , char* argv[]) {
    if(argc!=2) {
        printf("USAGE: %S , PORT  \n",argv[0]);
        fflush(stdout);
        exit(1);
    }
    signal(SIGSEGV, handler);   // install our handler
    int port = atoi(argv[1]);

    ServerTCP* tserver= new ServerTCP(port);
    WebServer* webserv= new WebServer();


    bool exit = false;
    while( true ) {

        pthread_t manageConnection_thread;
        puts("\n**********NEW CONNECTION**********\n");
        ConnServer* serverC=  tserver->acceptConn();
        ServerInfo* serverinfo = (ServerInfo*) malloc(sizeof(ServerInfo));
        serverinfo->server = tserver;
        serverinfo->webserver = webserv;
        serverinfo->serverConn =serverC;
        //show_params(serverinfo,"starting in while()",0);
        //srvConn->inviaServer(msg);

        if(pthread_create(&manageConnection_thread,
                          NULL,
                          manageConnection,
                          (void*) serverinfo)) {
            error("Error Creating server");
        }
        //show_params(serverinfo,"end of while()", manageConnection_thread);
        //srvConn->inviaServer(msg);
    }
    delete( tserver);
    delete( webserv);
    return 0;
}




void *manageConnection(void * serverinformation) {
    ServerInfo *serverinfo = (ServerInfo*)serverinformation;
    //show_params(serverinfo,"in thread()", pthread_self());
    ConnServer * srvConn = serverinfo->serverConn;
    char* answ;
    if(srvConn) {
        answ = (char*)srvConn->riceviServer();
    }
    if(!answ) {
        printf("finerichiesta\n");
        fflush(stdout);

        fineRichiesta(serverinfo);
    }
    printf("answ: %s\n", answ);
    fflush(stdout);
    char* file = serverinfo->webserver->getFile(answ);
    srvConn->inviaServer(file);
    fineRichiesta( serverinfo);
    free(answ);
    return NULL;
}
void fineRichiesta(ServerInfo * serverinformation) {
    printf("fine richiesta\n");
    fflush(stdout);
    puts("\n**********CONNECTION CLOSED**********\n");
    serverinformation->server->disconnect(serverinformation->serverConn);
    free(serverinformation);
    pthread_exit(NULL);
}
