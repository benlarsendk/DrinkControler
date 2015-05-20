#include "server.h"
#include "log.h"

Server::Server(Admin* inAdmin){
   admin = inAdmin;

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


void Server::connectionHandler(void * socket_desc){

    Logger::instance()->log("Client connected, worker started.");

    int sock = *(int*)socket_desc;
    int n;
    char myBuff[512];

    while( (n = recv(sock , myBuff , 512 , 0)) > 0 )
        {
             if(strcmp(buffer_,"EXIT")){
                 admin->parser(myBuff, sock);
                }
        }

    delete socket_desc;
}


void Server::start(){

    listen(sockfd_,5); // Lyt for incomming forbindelser. Den tager filedescriptoor og størrelsen
                      // for en backlog queue. 5 er max.
   Logger::instance()->log("Server started, listening for incomming connections.");
    /* accept blocker indtil en client skaber forbindelse til serveren.
     * Den vågnet når der kommer en ny forbindelse og returnerer en ny fil-descriptor
     * og kommunikation skal nu gøres med den nye. Andet agument er en reference til en pointer
     * med adressen til klienten. Sidste argument er bare størrelsen på strukturen*/
    std::cout << "Server will start listening now.." << std::endl;

    //for(;;)
    //{

    int *new_sock_;

    while(newsockfd_ = accept(sockfd_, (struct sockaddr *) &cli_addr, &clien_))
    {
        if (newsockfd_ < 0){
          error("Couldn't accept incomming connection");          
          }

        // Forbindelse accepteret

        new_sock_ = new int;
        *new_sock_ = newsockfd_;
        boost::thread(&Server::connectionHandler, this, new_sock_);

        // JOIN!!!! //
    }
}

Server::~Server()
{
    close(newsockfd_);
    close(sockfd_);
    //delete admin;
}

void Server::send(string in, int mysockfd){
    char data[512] = {};

    strcpy(data, in.c_str());
    int n = 0;

    n = write(mysockfd,data,strlen(data));
    if (n < 0)
        error("ERROR writing to socket");
    else Logger::instance()->log("Server wrote: " + in);
    memset(data, 0, 512);

}

void Server::error(char* error){

    string errorn = string(error);
    Logger::instance()->log(errorn);
    close(newsockfd_);
    close(sockfd_);
    exit(1);
}
