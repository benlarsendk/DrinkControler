#include "admin.h"
#include "guidummy.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>

Admin::Admin(Controller *controller)
{
   GUINF = controller;//
}

bool Admin::checkNameDrink(string namecheck)
{
    if(db.checkName(DRINK,namecheck)){
        return false; // findes allerede
    }
    else{
       return true;
    }

}

void Admin::getIngredientsName(vector <string> & currentIngredients)
{
    if(db.getIngredientsName(currentIngredients)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->printIngredients(currentIngredients);
    }
}


bool Admin::createDrink(Drink newDrink)
{
    if(db.createDrink(newDrink)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return false;
    }
    else return true;
}

void Admin::getDrinksName()
{
    vector<string> drinks;
    if(db.getDrinksName(drinks)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->printDrinks(drinks);
    }
}

map<string,string> Admin::checkStock()
{
    vector<string> ings;

    if(db.getIngredientsName(ings)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        map<string,string> error;
        return error;
    }


    map<string,string> stock; // Navn, mængde

    int fd = open("/dev/spidev", O_RDWR);
    u_int8_t cmd = 0x02;
    write(fd,&cmd,8); // stock state


    for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
        char tmpBuf[8];
        read(fd,tmpBuf,8);
        string tmp(tmpBuf);
        stock[*iter] = tmp;
    }

   return stock;

}


void Admin::getDrink(string name)
{
    Drink drink;
    if(db.getDrink(name,drink)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->printDrink(drink);
    }
}

void Admin::changeDrink(Drink drinktoedit)
{
    if(db.changeDrink(drinktoedit)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->print("OK");
    }
}

void Admin::deleteDrink(string todelete)
{
    if(GUINF->getConfirm()){
       if(db.remove(todelete,DRINK)!=0){
           GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
           return;
       }
       else{
           GUINF->print("The entry has been deleted.");
       }
    }
    else{
        GUINF->print("Delete canceled");
    }
}

bool Admin::checkNameIngredient(string name)
{
    return db.checkName(INGREDIENT,name);
}

bool Admin::checkContainer(int addr)
{
    if(db.checkContainerInUse(addr)){ //return true hvis findes
        return false;
    }
    else return true;
}

void Admin::createIngredient(string name, int addr)
{
    if(db.createIngredient(name,addr)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->print("Ingredient created");
    }
}

void Admin::getIngredientAddress(string ingredient)
{
    int addr;
    if(db.getIngAdress(ingredient,addr)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->printInfo(addr);
    }
}


void Admin::changeIngredientAddr(string name,int newAddr)
{
    if(db.changeIngrediensAddr(name,newAddr)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        GUINF->print("Ingredient changed");
    }
}

void Admin::deleteIngredient(string todelte)
{
    if(db.checkForUse(todelte)){
        GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
        return;
    }
    else{
        if(db.remove(todelte,INGREDIENT)!=0){
            GUINF->print("DB ERROR: " + getErrorPT(db.getLastError()));
            return;
        }
        else{
            GUINF->print("Ingredient deleted");
        }
    }
}

void Admin::orderDrinks(vector<string> drinks){
    if(GUINF->confirmOrder()){
    	
	int fd = open("/dev/spidev", O_RDWR);
        for(vector<string>::iterator i = drinks.begin(); i < drinks.end(); i++){

            u_int8_t cmd = 0x01;
            write(fd,&cmd,8); // Order state

            Drink current;
            int count = 0;
            //db.getDrink(*i,current);

            if(db.getDrink(*i,current) == 0){

                vector<int> ingredients;
                db.getAddress(current.name, ingredients);

                for (vector<int>::iterator x = ingredients.begin(); x < ingredients.end(); x++){
                    u_int8_t ing = *x;
                    u_int8_t amt = current.content[count].amount;


                    write(fd,&ing,8);
                    write(fd,&amt,8);
                    count++;

                }

                log.log("Ingredient: " + current.name + " has been written to PSoC");
                GUINF->print("Drink " + current.name + " Has been ordered");
                db.saveOrder(current.name);
            }
            else{
                GUINF->print("DB ERROR: " +getErrorPT(db.getLastError()));
                return;
            }
        }
    }
    else{
        GUINF->print("Order cancelled");
        return;
    }
}

void Admin::clean()
{
	int fd = open("/dev/spidev", O_RDWR);	
    u_int8_t clean = 0x03;
    char buff[8];
    char buff2[8];

    write(fd,&clean,8);

    while(read(fd,buff,8) == 0){
        sleep(1);
	}


    GUINF->print("Add water");
    while (!GUINF->confirmOrder()){
        sleep(1);
	}

    write(fd,&clean,8);
    while(read(fd,buff2,8) == 0){
        sleep(1);
	}
    GUINF->print("Done. Add original and confir.");
    while(!GUINF->confirmOrder()){
        sleep(1);
	}
	return;
	
}

string Admin::getErrorPT(int error)
{
    switch(error){
    case NO_ERRORS:
        return "NO_ERRORS";
        break;
    case DB_FAILED_TO_OPEN:
        return "DB_FAILED_TO_OPEN";
        break;
    case DB_QUERY_FAIL:
        return "DB_QUERY_FAIL";
        break;
    case DB_INGREDIENS_NOT_FOUND:
        return "DB_INGREDIENS_NOT_FOUND";
        break;
    case UNDEFINED_PARAMETER:
        return "UNDEFINED_PARAMETER";
        break;
    case DB_ENTRY_ALREADY_EXIST:
        return "DB_ENTRY_ALREADY_EXIST";
        break;
    case DB_DRINK_NOT_FOUND:
        return "DB_DRINK_NOT_FOUND";
        break;
    default:
        return "UNKNOWN ERROR";
        break;
    }
}
