#include "src/adminclient.h"
#include "src/guinf.h"
#include <iostream>



int main()
{
    cout << "****FAKE AD GUI****" << endl << endl;

    cout << "[*] Creating Controller and AC" << endl;
    Controller me;
    AdminClient AC(&me);


    map<string,string> stock = AC.checkStock();



    for(map<string,string>::iterator it = stock.begin(); it != stock.end(); it++){
        cout << "NAME: " << it->first << endl;
        cout << "AMT: " << it->second << endl << endl;
    }



/*
    cout << "Get ingrediens" << endl;
    vector<string> ings;
    AC.getIngredientsName(ings);

    for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
        cout << *iter << endl;
    }


    Drink vin;
    vin.name = "GayMister";
    vin.path ="/root/home/GayPic.png";
    vin.content[0].name="Cola";
    vin.content[0].amount=35;

    AC.createDrink(vin);



    vector<string> drinks = AC.getDrinksName();

    for (vector<string>::iterator iter = drinks.begin(); iter != drinks.end(); iter++){
        cout << *iter << endl;
    }

    Drink get = AC.getDrink("Tequilla Sunrise");

    cout << "!Drink!" << endl;
    cout << "NAME: " << get.name << endl;
    cout << "PATH: " << get.path << endl;

    for (int i = 0; i < 5; i++){
        cout << "ING: " << get.content[i].name << endl;
        cout << "AMT: "<< get.content[i].amount << endl;
    }

    cout << "LET's CHANGE DRI!!NK!!" << endl;

    get.content[2].name = "Negersperm";
    get.content[2].amount = 54;

    AC.changeDrink(get);


    Drink get2 = AC.getDrink("Tequilla Sunrise");

    cout << "!Drink!" << endl;
    cout << "NAME: " << get2.name << endl;
    cout << "PATH: " << get2.path << endl;

    for (int i = 0; i < 5; i++){
        cout << "ING: " << get2.content[i].name << endl;
        cout << "AMT: "<< get2.content[i].amount << endl;
    }



    cout << "FUCKING DELEETE" << endl;
    AC.deleteDrink("GayMister");


    cout << "WALLAH CHECK FOR ROM!" << endl;
    AC.checkNameIngredient("Rom");


    cout << "FUCKING TEST FOR LOL  CONTAINERE - TRUE HVIS FINDES" << endl;
    if(AC.checkContainer(65)){
        cout << "65 findes" << endl;
    }
    else cout << "Fucking a.." << endl;


    cout << "TEST af createIngredient" << endl;

    AC.createIngredient("Rumænerbræk",999);

    cout << "WAUWUW! Test af get addr." << endl;
    cout << "Addres for rumænerbræk: " << AC.getIngredientAddress("Rumænerbræk") << endl;

    cout << "CHANGE THE FUCKING ADDR TO 239" << endl;
    AC.changeIngredientAddr("Rumænerbræk",239);
    cout << "Addres for rumænerbræk: " << AC.getIngredientAddress("Rumænerbræk") << endl;

    cout << "FUCK RUMÆNERBRÆK!! DELETE!" << endl;
    AC.deleteIngredient("Rumænerbræk");

    cout << "WALLAH CHECK FOR RUMÆNERBRÆK!" << endl;
    if(AC.checkNameIngredient("Rumænerbræk")){
        cout << "YEAH BITCH!!!" << endl;
    }


    cout << "MOTHERFUCKING CLEANUP! YEARH!" << endl;
/*
    AC.clean();

    sleep(2);

*/    



}
