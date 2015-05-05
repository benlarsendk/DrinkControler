#ifndef ADMIN
#define ADMIN

#include <iostream>
#include <string>
#include "src/dbif.h"
#include <vector>
#include "src/drink.h"
#include "src/log.h"
#include "src/guidummy.h"

using namespace std;

class Admin
{
public:
    Admin();
    void checkNameDrink(string namecheck);
    void orderDrinks(vector<string> drinks);
    void getIngredientsName(vector <string> & currentIngredients);
    bool createDrink(Drink newDrink);
    void getDrinksName();
    void getDrink(string name);
    void changeDrink(Drink drinktoedit);
    void deleteDrink(string todelte);
    bool checkNameIngredient(string name);
    bool checkContainer(int addr);
    void createIngredient(string name, int addr);
    void getIngredientAddress(string ingredient);
    void changeIngredientAddr(string name,int newAddr);
    void deleteIngredient(string todelte);
	void clean();
    string getErrorPT(int);

private:
    DatabaseIF db;
    Logger log;
    GUI GUINF;

};
#endif // ADMIN

