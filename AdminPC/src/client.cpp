#include "client.h"
#include <iostream>

Client::Client()
{
    portno = 7913;
    server = gethostbyname("10.9.8.2");

}

void Client::send(std::string dataIn){

    std::cout << "Trying to send: " << dataIn << std::endl;

    char data[2048];
    strcpy(data,dataIn.c_str());

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

    bzero(data,2048);

}

std::string Client::recieve(){
    int n;
    char bufferIn[2048];

    n = read(sockfd,bufferIn,2048);

    if (n < 0){
        std::cout << "ERROR: " << n << std::endl;
    }

    std::string data(bufferIn);
    //std::cout << "Client got " << data << std::endl;
    memset(bufferIn,0,512);
    return data;


}

Client::~Client()
{
    close(sockfd);
}

void Client::error(std::string err){
    std::cout << "Error: " << err << std::endl;
}
