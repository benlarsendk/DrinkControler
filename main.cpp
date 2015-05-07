#include <QCoreApplication>
#include "src/admin.h"
#include "src/dbif.h"
#include "src/log.h"
#include "src/guidummy.h"
#include "string.h"

using namespace std;

int main(int argc, char *argv[])
{
    Admin control;
    /*
    /* Simple test

    vector<string> Drinks;
    Drinks.push_back("Vodka og Redbull");
    Drinks.push_back("Cola and Soya for rich people");
    control.orderDrinks(Drinks);
*/

    map<string,string> test = control.checkStock();

    for (map<string,string>::const_iterator iter = test.begin(); iter != test.end(); iter++){
        cout << "Ingrediens: " << iter->first << endl << "Mængde: " << iter->second << endl;
    }
    return 0;
}


