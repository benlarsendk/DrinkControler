#include <iostream>
#include "client.h"

int main()
{

    char data[256];
    Client* tmpClien = new Client;

    for (int i = 0; i < 5; i++){

        std::cout << "Indtast data: "   ;
        std::cin >> data;
        std::cout << std::endl;
        tmpClien->send(data);

    }

    tmpClien->recieve();

    delete tmpClien;




}
