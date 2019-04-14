#ifndef __WEBSERVER_HPP
#define __WEBSERVER_HPP

#define BUFSIZE 4096
#define DIRECTORY "../www"
#define MAX_CONNECTIONS 50
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "Address.hpp"
#include "../lib/mylib.h"
#include "sqlite3.h"
#include <regex.h>
using namespace std;
/********************************************
 * ******This is WEBSERVER TCP class*********
 * ******************************************/
class WebServer {
private:
    bool matched=false;
public:
    WebServer();
    ~WebServer();
    char* getFile(char* httpReq);
    char* makeReply(FILE* fp);
    bool readTaggedLine(FILE * fp,FILE * fout);
    char* dataFromDb(char* dbPath,char* query);
    char* requestedFileName(char* httpRequest);
    char* get_db_query(char* line);

};
/******************************/
WebServer::WebServer()
{}
/******************************/
char* WebServer::getFile(char* httpReq)
{
    int i = 0;
    char* noSuchFile = "HTTP/1.1 404 Not Found\n"
                       "Server: thisIsSinghsLazyServer/0.1.1\n"
                       "Content-Type: text/html\n"
                       "Content-Length: 21\n"
                       "Connection: close\n"
                       "\n"
                       "<h1>404 Not Found</h1>";

    FILE* fp = NULL;

    //char** requestfirstline = split(filename, ' ');
    //char* extractfile = requestfirstline[1];

    char* extractfile = requestedFileName(httpReq);
    printf("%s\n",extractfile);
    fflush(stdout);
    //free(requestfirstline);

    if (strcmp(extractfile, "/")) {
        int i = intContains(extractfile, '/');
        if (i == 1) {
            //extractfile = removeChar(extractfile, '/');
            extractfile=concat(DIRECTORY,extractfile);
            fp = fopen(extractfile, "r");
            free(extractfile);
            if (fp) {
                char* v = makeReply(fp);
                fclose(fp);
                return v;
            }
            else {
                return noSuchFile;
            }
        }
        else {
            int lenstring = strlen(extractfile);
            if (*(extractfile + lenstring - 1) != '/') {
                //extractfile = removeFirstChar(extractfile);
                extractfile=concat(DIRECTORY,extractfile);
                fp = fopen(extractfile, "r");
                free(extractfile);
                if (fp) {
                    char* v = makeReply(fp);
                    fclose(fp);
                    return v;
                }
                else {
                    return noSuchFile;
                }
            }
            else {
                //extractfile = removeFirstChar(extractfile);
                extractfile=concat(DIRECTORY,extractfile);

                char* pathfile = concat(extractfile, "index.html");
                fp = fopen(pathfile, "r");
                free(extractfile);
                if (fp) {
                    char* v = makeReply(fp);
                    fclose(fp);
                    return v;
                }
                else {
                    return noSuchFile;
                }
            }
        }
    }
    else {

        fp = fopen("../www/index.html", "r");
        free(extractfile);
        if (fp) {

            char* v = makeReply(fp);
            fclose(fp);
            return v;
        }
        else {
            return noSuchFile;
        }
    }
}
/******************************/
char* WebServer::makeReply(FILE* fp)
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    // Determine file size
    /*fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    char* where = (char*)malloc(sizeof(char) * size);
    rewind(fp);
    fread(where, sizeof(char), size, fp);*/
    char* fileoutpath = (char*)malloc(sizeof(char)*100);
    sprintf(fileoutpath,"../files/file%d.txt",pthread_self());
    FILE* fout = fopen(fileoutpath,"w");
    char* ret;
    char reply[155];
    sprintf(reply, "HTTP/1.1 200 OK\n"
            "Date: %s GMT\n"
            "Server: thisIsSinghsLazyServer/0.1.1\n"
            "Content-Type: text/html\n"
            "Connection: close\n"
            "\n",
            dt);
    fprintf(fout,"%s",reply);

    bool result = readTaggedLine(fp,fout);
    if(result){
	fclose(fout);
	return fileoutpath;
    }
    fclose(fout);
    return NULL;
}
/******************************/
bool WebServer::readTaggedLine(FILE * fp,FILE * fout) {
  char * line = NULL;
  size_t len = 0;
  char * result;
  while (getline( &line, & len, fp) != -1) {
    char * returnedValue = get_db_query(line);
    if (matched) {
      char ** vectParams = split(returnedValue, ';');
      //char* s =removeTag(line, strlen(tag));
      //char *c =dataFromDb("../serverTcp/Testing.db",s);
      char * c = dataFromDb(vectParams[0], vectParams[1]);
      printf("Table - \n%s\n", c);
      fflush(stdout);
      fprintf(fout,"%s",c);
      //str.append(c);
      free(c);
      matched=false;
     
      //free(s);

    } else {
      fprintf(fout,"%s",line);

    }
}

  return true;
}
/******************************/
char* WebServer::dataFromDb(char* dbPath, char* query) {
    sqlite3* DB;
    sqlite3_stmt *stmt;
    int exit = 0;
    exit = sqlite3_open(dbPath, &DB);
    std::string str;
    if (exit) {
        printf("Error open DB %s",sqlite3_errmsg(DB));
        fflush(stdout);
        return NULL;
    }
    else
        printf("Opened Database Successfully!\n");
    fflush(stdout);
    printf("Performing query...\n");
    fflush(stdout);
    //style='color: #040404;width: 100%;text-align: center;background: white;padding: 10px;'>
    
    if (sqlite3_prepare_v2(DB, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Prepare failure: %s",sqlite3_errmsg(DB));
        fflush(stdout);
    	return NULL;
    }

str.append("<table class='table table-bordered'>");
    bool cols =true;
    printf("Got results:\n");
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        int i;
        int num_cols = sqlite3_column_count(stmt);
        if(cols) {
            str.append("<tr>");
            for (i = 0; i < num_cols; i++) {
                str.append("<th>");
                str.append(sqlite3_column_name(stmt, i));
                str.append("</th>");
            }
            str.append("</tr>");
            cols=false;
        }


        str.append("<tr>");
        for (i = 0; i < num_cols; i++)
        {
            switch (sqlite3_column_type(stmt, i))
            {
            case (SQLITE3_TEXT):
                str.append("<td>");
                str.append((char*)sqlite3_column_text(stmt, i));
                str.append("</td>");
                printf("%s, ", sqlite3_column_text(stmt, i));
                break;
            case (SQLITE_INTEGER):
                str.append("<td>");
                str.append(std::to_string(sqlite3_column_int(stmt, i)));
                str.append("</td>");
                printf("%d, ", sqlite3_column_int(stmt, i));
                break;
            case (SQLITE_FLOAT):
                str.append("<td>");
                str.append(std::to_string(sqlite3_column_double(stmt, i)));
                str.append("</td>");
                printf("%g, ", sqlite3_column_double(stmt, i));
                break;
            default:
                break;
            }
        }
        printf("\n");
        fflush(stdout);

    }
    str.append("</table>");
    char*  ret= strdup(str.c_str());

    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    return ret;
}
/******************************/
char* WebServer::requestedFileName(char* httpRequest) {
    //char * source = "GET /favicon.ico HTTP/1.1";
    const char * regexString = "^GET\\s([^\\?\\*<>|]+)\\sHTTP";
    size_t maxMatches = 2;
    size_t maxGroups = 2;
    char* ret = (char*) malloc(sizeof(char)*100);
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    //char * cursor;
	
    if (regcomp(&regexCompiled,regexString, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        fflush(stdout);
        return NULL;
    }


    if (!regexec(&regexCompiled,httpRequest, maxGroups, groupArray, 0)) {
        char cursorCopy[strlen(httpRequest) + 1];
        strcpy(cursorCopy, httpRequest);
        cursorCopy[groupArray[1].rm_eo] = '\0';
        printf("Match %s\n",
            cursorCopy + groupArray[1].rm_so);
        sprintf(ret,"%s",cursorCopy + groupArray[1].rm_so);
    }


    regfree(&regexCompiled);

    return ret;
}
char* WebServer::get_db_query(char* line){
    //char * source = "GET /favicon.ico HTTP/1.1";
    
    const char * regex = "<sql\\s+db=(.+?)\\s+query=(.+;)\\s*\\\\>";
    size_t maxMatches = 3;
    size_t maxGroups = 3;
    char* ret = (char*) malloc(sizeof(char)*100);

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    //char * cursor;
    //printf("%s\n\n",line);
   // fflush(stdout);
    if (regcomp(&regexCompiled,regex, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        fflush(stdout);
        return NULL;
    };

    if (!regexec(&regexCompiled,line, maxGroups, groupArray, 0)) {
        char cursorCopy[strlen(line) + 1];
        strcpy(cursorCopy, line);
        cursorCopy[groupArray[1].rm_eo] = '\0';
        cursorCopy[groupArray[2].rm_eo] = '\0';
        printf("Match %s\n",
            cursorCopy + groupArray[1].rm_so);
        printf("Match %s\n",
            cursorCopy + groupArray[2].rm_so);
            fflush(stdout);
        matched=true;
        sprintf(ret,"%s;%s",cursorCopy + groupArray[1].rm_so,cursorCopy + groupArray[2].rm_so);
    }

    regfree(&regexCompiled);
    
    return ret;

}

/******************************/
WebServer::~WebServer()
{}



#endif //__WEBSERVER_HPP
