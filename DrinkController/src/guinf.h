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
    void print(std::string);
    bool confirmOrder()
    {
        return true;
    }
    void printDrinks(vector<std::string>);

private:

};


#endif // GUINFA


