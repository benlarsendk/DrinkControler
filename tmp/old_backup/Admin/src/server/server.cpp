#include "server.h"
#include "tmpadmin.h"
#include <string>

Server::Server(){
    Admin* admin = new Admin;

    portno_ = 7913;

     // Indeholder en internet addresse (den er i in.h)

    /* Lave en ny socket
     * AF_INTET er sockets adresse domæne, og når det er over "internettet" er det AF_INET.
     * SOCK er socket-typen. SOCK_STREAM betyder at der læses kontinuært fra en fil eller rør.
     * Er protokollen. Når den er 0, vælger systemet selv (TCP for stream)
     */
    sockfd_ = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd_ < 0){
      error("Socket error: Can't open  socket");
    }

    /*Sætter alle værdier i en buffer til 0
     * Det første er en pointer til til bufferen, og den anden er størrelsen.
     * Den initialiserer altså server-adressen til 0'ere.
     */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; // Skal altid være AF_INET
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Ipadressen på hosten.
    serv_addr.sin_port = htons(portno_); // Portnummeret, men skal konverteres til network byte order

    /* Binder en socket til en adresser
     * 1. Socket file desciptor
     * 2. Adressen den skal bindes til
     * 3. Størrelsen af adressen
     */
    if(bind(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr))  < 0){
      error("Couldn't bind");
    }
}

void Server::start(){

    listen(sockfd_,5); // Lyt for incomming forbindelser. Den tager filedescriptoor og størrelsen
                      // for en backlog queue. 5 er max.

    /* accept blocker indtil en client skaber forbindelse til serveren.
     * Den vågnet når der kommer en ny forbindelse og returnerer en ny fil-descriptor
     * og kommunikation skal nu gøres med den nye. Andet agument er en reference til en pointer
     * med adressen til klienten. Sidste argument er bare størrelsen på strukturen*/
    std::cout << "Server will start listening now.." << std::endl;

    for(;;)
    {

        newsockfd_ = accept(sockfd_, (struct sockaddr *) &cli_addr, &clien_);
        if (newsockfd_ < 0){
          error("Couldn't accept incomming connection");
        }

        /********** Nu er der kommet forbindelse ***********/

        bzero(buffer_,256); //Sætter bufferen til 0.
        int n = 0;
        n = read(newsockfd_,buffer_,255); // Read!
        if (n < 0){
          error("Couln't read from scoket");
        }
        /*Fortsætter med at læse indtil den får exit*/
        if(strcmp(buffer_,"EXIT")){
            admin->parser(buffer_);
        }
        else break;

    }
}

Server::~Server()
{
    close(newsockfd_);
    close(sockfd_);
    delete admin;
}

void Server::send(char* data){
    int n;

    n = write(newsockfd_,data,strlen(data));
         if (n < 0) error("ERROR writing to socket");
}

void Server::error(char* error){

    std::cout << "Error: " << error << std::endl;
    close(newsockfd_);
    close(sockfd_);
    exit(1);
}
