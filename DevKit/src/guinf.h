#ifndef GUINFA
#define GUINFA

#include <vector>
#include "drink.h"
#include <iostream>
#include <string.h>


class Controller
{
public:
    Controller(){}
    void print(std::string in){
        std::cout << in << std::endl;
    }

    bool confirmOrder()
    {
        return true;
    }
    void printDrinks(vector<std::string> in){
        for (vector<std::string>::iterator iter = in.begin(); iter != in.end(); iter++){
            std::cout << *iter << std::endl;
        }
    }

private:

};


#endif // GUINFA


