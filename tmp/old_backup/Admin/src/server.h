#ifndef SERVER
#define SERVER

#include "src/admin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>      // Definitioner på datatyper i systemkald.
#include <sys/socket.h>     // Definitioner for sockets
#include <netinet/in.h>     // strukturer for internet domain adresser
#include <boost/thread.hpp>
class Admin;

class Server
{
public:
    Server();
    Server(Admin*);
    ~Server();
    void start();
    void send(std::string in, int sock);
    void error(char*);

private:
     int sockfd_, newsockfd_;
     int portno_;
     void connectionHandler(void *);
     socklen_t clien_;
     char buffer_[512];
     struct sockaddr_in serv_addr, cli_addr;
     Admin* admin;

};
#endif // SERVER

