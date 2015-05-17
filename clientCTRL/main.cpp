#include "src/adminclient.h"
#include "src/guinf.h"

int main()
{
    cout << "****FAKE AD GUI****" << endl << endl;

    cout << "[*] Creating Controller and AC" << endl;
    Controller me;
    AdminClient AC(&me);


    cout << "[*] Requesting drinks list" << endl;
    usleep(500);
   vector<string> test = AC.getDrinksName();

   for (vector<string>::iterator iter = test.begin(); iter != test.end(); iter++){

       cout << *iter << endl;
   }


   cout << endl << "[*] Requesting Ingredients list" << endl;
   usleep(500);
   vector<string> ings;
   AC.getIngredientsName(ings);

   for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){

       cout << *iter << endl;
   }


   cout << endl << "[*] Creating a drink [Kaffe | Kaffepulver | 25 | vand | 30 | LOL/PATH]" << endl;
   usleep(500);
   Drink kaffe;
   kaffe.name =" Kaffe";
   kaffe.content[0].name="Kaffepulver";
   kaffe.content[0].amount=25;
   kaffe.content[1].name="Vand";
   kaffe.content[1].amount=30;
   kaffe.path="LOL/PATH";

   AC.createDrink(kaffe);

   usleep(500);
   cout << "[*] Seeing if it worked..." << endl;

   vector<string> test2 = AC.getDrinksName();
   for (vector<string>::iterator iter = test2.begin(); iter != test2.end(); iter++){

       cout << *iter << endl;
   }





}
