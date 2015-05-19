#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


class Client
{
public:
    Client();
    void send(std::string data);
    void error(std::string err);
    std::string recieve();
    ~Client();
private:
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

};

#endif // CLIENT_H
