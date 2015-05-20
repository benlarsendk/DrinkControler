#include "client.h"
#include <iostream>

Client::Client()
{
    portno = 7913;
    server = gethostbyname("10.9.8.2");

}

void Client::send(char *data){

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("Coulnd't' open socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("Coulnd't connect");


    int n;
    n = write(sockfd,data,strlen(data));
    if (n < 0)
            error("ERROR writing to socket");

    bzero(data,256);

}

void Client::recieve(){
    int n;
    n = read(sockfd,bufferIn,255);
       if (n < 0)
            std::cout << "ERROR: " << n << std::endl;
       printf("%s\n",bufferIn);
}

Client::~Client()
{
    close(sockfd);
}

void Client::error(char *err){
    std::cout << "Error: " << err << std::endl;
}
