#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



class Client
{
public:
    Client();
    void send(char* data);
    void error(char* err);
    void recieve();
    ~Client();
private:
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char bufferIn[256];

};

#endif // CLIENT_H
