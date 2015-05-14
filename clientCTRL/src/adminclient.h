#ifndef ADMINCLIENT
#define ADMINCLIENT

#include <iostream>
#include <src/client.h>
#include <src/drink.h>
#include <src/guinf.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>

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


class AdminClient
{
public:
    AdminClient(Controller*);
    ~AdminClient();
    void changeDrink(Drink drinktoedit);
    void clean();
    void clean_water();
    void getIngredientsName(vector <string> & currentIngredients);
    bool deleteDrink(string todelte);
    bool checkNameIngredient(string name);
    bool checkContainer(int addr);
    bool changeIngredientAddr(string name,int newAddr);
    bool deleteIngredient(string todelte);
    void createIngredient(string name, int addr);
    int getIngredientAddress(string ingredient);
    bool checkNameDrink(string namecheck);
    void createDrink(Drink newDrink);
    std::string getErrorPT(int);
    std::vector<string> getDrinksName();
    std::map<string,string> checkStock();
    Drink getDrink(string name);

private:
    Client* client;
    Controller* GUINF;
};

#endif // ADMINCLIENT

