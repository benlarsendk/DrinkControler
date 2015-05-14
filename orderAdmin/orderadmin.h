#ifndef ORDERADMIN_H
#define ORDERADMIN_H
#include <iostream>
#include <string>
#include "src/dbif.h"
#include <vector>
#include <map>
#include "src/drink.h"
#include "src/log.h"
#include "guinf.h"

using namespace std;

class orderAdmin
{
public:
    orderAdmin(Controller*);
    ~orderAdmin();

    void getDrinksName();
    void orderDrinks(vector<string> drinks);
    map<string,string> checkStock();
    string getErrorPT(int);

private:
    Controller* GUINF;
    DatabaseIF db;
    Logger log;
};

#endif // ORDERADMIN_H
