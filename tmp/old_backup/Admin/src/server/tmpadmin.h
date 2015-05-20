#ifndef TMPADMIN
#define TMPADMIN

#include <iostream>
class Admin
{
public:
    Admin(){
        std::cout << "Admin created" << std::endl;
    }

    void parser(char* data){

        std::cout << "Im doing something with the data: " << data << std::endl;

    }

};


#endif // TMPADMIN

