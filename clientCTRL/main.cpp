#include "src/adminclient.h"
#include "src/guinf.h"

int main()
{
    Controller erik;

    AdminClient LOL(&erik);

   // LOL.listen();
   vector<string> test = LOL.getDrinksName();

   for (vector<string>::iterator iter = test.begin(); iter != test.end(); iter++){

       cout << *iter << endl;
   }


}
