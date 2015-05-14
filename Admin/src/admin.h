#ifndef ADMIN
#define ADMIN

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "src/drink.h"
#include "src/dbif.h"
#include "src/log.h"
#include "src/server.h"
#include "src/guidummy.h"
class Server;

using namespace std;
enum
{
    CHECKNAMEDRINK = 0,
    CHECKSTOCK = 1,
    GETINGREDIENTSNAME = 3,
    CREATEDRINK = 4,
    GETDRINKSNAME = 5,
    GETDRINK = 6,
    CHANGEDRINK = 7,
    DELETEDRINK = 8,
    CHECKNAMEINGREDIENT = 9,
    CHECKCONTAINER = 10,
    CREATEINGREDIENT = 11,
    GETINGREDIENTADDR = 12,
    CHANGEINGREDIENTADDR =13,
    DELETEINGREDIENT = 14,
    CLEAN = 15,
    GETERROR = 16,
    CLEAN_WATER = 17
};

class Admin
{
public:
    Admin(Controller*);
    ~Admin();
    //Admin(Controller*);
    string checkNameDrink(string namecheck);
    void decode(string, vector<string> &);
    map<string,string> checkStock();
    void getIngredientsName(vector <string> & currentIngredients);
    string createDrink(Drink newDrink);
    vector<string> getDrinksName();
    Drink getDrink(string name);
    void changeDrink(Drink drinktoedit);
    bool deleteDrink(string todelte);
    bool checkNameIngredient(string name);
    bool checkContainer(int addr);
    void createIngredient(string name, int addr);
    int getIngredientAddress(string ingredient);
    bool changeIngredientAddr(string name,int newAddr);
    bool deleteIngredient(string todelte);
    void clean();
    void clean_water();
    void parser(char*);
    string getErrorPT(int);

private:
    DatabaseIF db;
    Logger log;
    bool confFlag;
    Controller* GUINF;
    Server* server;
    //Controller *GUINF;

};



#endif // ADMIN

