#ifndef ORDERADMIN_H
#define ORDERADMIN_H
#include <iostream>
#include <string>
#include "dbif.h"
#include <vector>
#include <map>
#include "drink.h"
#include "log.h"
#include <queue>
#include <boost/thread.hpp>
#include "guinf.h"

using namespace std;

class orderAdmin
{
public:
    orderAdmin(Controller*, DatabaseIF*);
    ~orderAdmin();
    vector<string> getDrinksName();
    Drink getDrink(string name);
    void orderDrinks(vector<string> drinks);
    map<string,string> checkStock();
    string getErrorPT(int);
    boost::thread* worker;
private:
    Controller* GUINF;
    queue<vector<string> > orders; // New
    boost::mutex mtx;
    boost::condition_variable bell;
    void handleOrder();
    DatabaseIF* db;
};

#endif // ORDERADMIN_H
