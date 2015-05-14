#include "orderadmin.h"
#include "dbif.h"
#include "log.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>

orderAdmin::orderAdmin(Controller* inGui)
{
    GUINF = inGui;
}

orderAdmin::~orderAdmin()
{
    delete GUINF;
}

void orderAdmin::getDrinksName()
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

void orderAdmin::orderDrinks(vector<string> drinks){
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

string orderAdmin::getErrorPT(int error)
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


map<string,string> orderAdmin::checkStock()
{
    vector<string> ings;

    if(db.getIngredientsName(ings)!=0){
        log.log("DB ERROR: " + getErrorPT(db.getLastError()));
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
