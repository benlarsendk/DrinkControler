#ifndef GUIDUMMY
#define GUIDUMMY

#include <vector>
#include "src/drink.h"
class GUI
{
public:
   void printIngredients(vector<string>);
   void printDrinks(vector<string>);
   void print(string);
   void printDrink(Drink);
   bool getConfirm();
   void printInfo(int adress);
   bool confirmOrder();
};
#endif // GUIDUMMY

