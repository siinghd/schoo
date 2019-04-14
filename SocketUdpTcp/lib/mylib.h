#include <stdio.h>
#include <stdlib.h>

#ifndef MYLIB_H
#define MYLIB_H

#define MAXSTR 4096
#define MAXVECT 1024
#define IP_LOOP "127.0.0.1"
#define IP_DHCP "0.0.0.0"
int lenStr(char* s);
char* cpyStr(char* s);
char* uppercase(char* s);
char* lowercase(char* s);
char* findChar(char* s, char c);
char* concat(char* s1, char* s2);
char* revStr(char* s);
int str2int(char* s);
char* int2str(int n);
char* inputStr();
void outputStr(char* s);
char* removeChar(char* s, char c);
char* addChar(char* s, char c);
int lenVett(char** v);
char** dupVett(char** v);
char** split(char* s, char c);
void freeVett(char** v);
void showVett(char** v);
char* readLine(FILE* fp);
int cmpStr(char* s1, char* s2);

void error(char* msg)
{
    perror(msg);
    exit(1);
}

void swap(char* a, char* b)
{
    char c;
    c = *a;
    *a = *b;
    *b = c;
}

int lenStr(char* s)
{
    int ret;
    for (ret = 0; *(s + ret); ret++) {
    }
    return ret;
}

char* cpyStr(char* s)
{
    char *tmp, *ret = (char *)malloc(sizeof(char) * (lenStr(s) + 1));
    for (tmp = ret;* tmp = *s; s++, tmp++) {
    }
    return ret;
}

char* uppercase(char* s)
{
    char* tmp;
    for (tmp = s; *s; s++)
        if ((*s >= 'a') && (*s <= 'z'))
            *s += 'A' - 'a';
    return tmp;
}

char* lowercase(char* s)
{
    char* tmp;
    for (tmp = s; *s; s++)
        if ((*s >= 'A') && (*s <= 'Z'))
            *s += 'a' - 'A';
    return tmp;
}

char* findChar(char* s, char c)
{
    for (; *s; s++)
        if (*s == c)
            return s;
    return NULL;
}

char* concat(char* s1, char* s2)
{
    char* tmp;
    char* ret = (char*)malloc(sizeof(char) * (lenStr(s1) + lenStr(s2) + 1));
    for (tmp = ret;* tmp = *s1; s1++, tmp++) {
    }
    for (;* tmp = *s2; s2++, tmp++) {
    }
    return ret;
}

char* revStr(char* s)
{
    char* s1 = s;
    char* s2 = s1 + lenStr(s1) - 1;
    while (s1 < s2)
        swap(s1++, s2--);
    return s;
}

int str2int(char* s)
{
    int ret;
    for (ret = 0; *s; s++)
        ret = ret * 10 + *s - '0';
    return ret;
}

char* int2str(int n)
{
    char ret[MAXSTR + 1];
    int i;
    for (i = 0; n; n /= 10, i++)
        ret[i] = '0' + (n % 10);
    ret[i] = '\0';
    return revStr(cpyStr(ret));
}

char* inputStr()
{
    char ret[MAXSTR + 1];
    int i;
    for (i = 0; (ret[i] = getchar()) != '\n'; i++) {
    }
    ret[i] = '\0';
    return cpyStr(ret);
}

void outputStr(char* s)
{
    for (; *s; s++)
        putchar(*s);
}

char* removeChar(char* s, char c)
{
    char* ret = (char*)malloc(sizeof(char) * (lenStr(s)));
    char* tmp = ret;
    int i;
    for (i = 0; *s; s++)
        if (*s != c)
            ret[i++] = *s;
    ret[i] = '\0';
    return cpyStr(tmp);
}

char* addChar(char* s, char c)
{
    char* ret = (char*)malloc(sizeof(char) * (lenStr(s) + 2));
    int i;
    for (i = 0; ret[i] = *s; s++, i++) {
    }
    ret[i++] = c;
    ret[i] = '\0';
    return cpyStr(ret);
}

int lenVett(char** v)
{
    int ret;
    for (ret = 0; *v; v++, ret++) {
    }
    return ret;
}

char** dupVett(char** v)
{
    char** ret = (char**)malloc(sizeof(char*) * (lenVett(v) + 1));
    for (char **tmp = ret; *tmp = *v; v++, tmp++) {
    }
    return ret;
}

char** split(char* s, char c)
{
    char* ret[MAXVECT + 1];
    int i;
    char* start;
    for (start = s, i = 0; *s; s++)
        if (*s == c) {
            *s = '\0';
            ret[i++] = cpyStr(start);
            *s = c;
            start = s + 1;
        }
    ret[i++] = cpyStr(start);
    ret[i] = NULL;
    return dupVett(ret);
}

void freeVett(char** v)
{
    for (char** tmp = v; *tmp; tmp++)
        free(*tmp);
    free(v);
}

void showVett(char** v)
{
    for (; *v; v++)
        printf("%s\n", *v);
}

char* readLine(FILE* fp)
{
    char buffer[MAXSTR + 1];
    int i = 0;
    while (((buffer[i] = fgetc(fp)) != '\n') && (buffer[i] != EOF))
        i++;
    if (i == 0)
        return NULL;
    buffer[i] = '\0';
    return cpyStr(buffer);
}

int cmpStr(char* s1, char* s2)
{
    if (*s1 < *s2)
        return -1;
    if (*s1 > *s2)
        return +1;
    if (*s1)
        return cmpStr(++s1, ++s2);
    return 0;
}

#endif /* MYLIB_H */
