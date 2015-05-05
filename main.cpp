#include <QCoreApplication>
#include "src/admin.h"
#include "src/dbif.h"
#include "src/log.h"
#include "src/guidummy.h"
#include "string.h"


int main(int argc, char *argv[])
{
    Admin control;

    vector<string> Drinks;
    Drinks.push_back("Vodka og Redbull");
    Drinks.push_back("Cola and Soya for rich people");
    control.orderDrinks(Drinks);

    return 0;
}


