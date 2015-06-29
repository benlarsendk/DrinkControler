#include "orderadmin.h"
#include "dbif.h"
#include "log.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>

orderAdmin::orderAdmin(Controller* inGui, DatabaseIF* dbI)
{
    GUINF = inGui;
    db = dbI;
    worker = new boost::thread(&orderAdmin::handleOrder, this);
}

orderAdmin::~orderAdmin()
{
    delete GUINF;
    delete worker;

}

vector<string> orderAdmin::getDrinksName()
{
    vector<string> drinks;
    if(db->getDrinksName(drinks)!=0){
        GUINF->print("DB ERROR: " + getErrorPT(db->getLastError()));
        return drinks;
    }
    else{
        return drinks;
    }
}

void orderAdmin::orderDrinks(vector<string> drinks){


    if(GUINF->confirmOrder()){
        boost::mutex::scoped_lock scoped_lock(mtx);
        orders.push(drinks);
        bell.notify_all();
        }

    else{
        GUINF->print("Order cancelled");
        return;
        }
}

void orderAdmin::handleOrder()
{
    for(;;)
    {
        boost::mutex::scoped_lock scoped_lock(mtx);
        while(orders.empty())
            bell.wait(scoped_lock);

        vector<string> drinks = orders.front();
        orders.pop();
        scoped_lock.unlock();

        int fd = open("/dev/spidev", O_RDWR);
        for(vector<string>::iterator i = drinks.begin(); i < drinks.end(); i++){

                u_int8_t cmd[] = "1";
                write(fd,cmd,8); // Order state

                Drink current;
                int count = 0;

                if(db->getDrink(*i,current) == 0){

                    vector<int> ingredients;
                    db->getAddress(current.name, ingredients);

                    for (vector<int>::iterator x = ingredients.begin(); x < ingredients.end(); x++){

                        int ing = *x;
                        int amt = current.content[count].amount;

                        stringstream strs;
                        stringstream strs1;
                        strs << ing;
                        string temp_str = strs.str();
                        char* inga = (char*) temp_str.c_str();

                        strs1 << amt;
                        string temp_str1 = strs1.str();
                        char* amta = (char*) temp_str1.c_str();

                        write(fd,inga,8);
                        write(fd,amta,8);
                        count++;
                    }

                    Logger::instance()->log("Ingredient: " + current.name + " has been written to PSoC");
                    GUINF->print("Drink " + current.name + " Has been ordered");
                    db->saveOrder(current.name);
                }
                else{
                    cout << "Error " << endl;
                    GUINF->print("DB ERROR: " +getErrorPT(db->getLastError()));
                }
            }
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

Drink orderAdmin::getDrink(string name)
{
    Drink drink;
    if(db->getDrink(name,drink)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return drink;
    }
    else{
        return drink;
    }
}

map<string,string> orderAdmin::checkStock()
{
    vector<string> ings;

    if(db->getIngredientsName(ings)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        map<string,string> error;
        return error;
    }


    map<string,string> stock; // Navn, mængde

    int fd = open("/dev/spidev", O_RDWR);
    u_int8_t cmd[] = "2";
    write(fd,cmd,8); // stock state
    sleep(3);


    for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
        char tmpBuf[8];
        read(fd,tmpBuf,8);
        string tmp(tmpBuf);
        stock[*iter] = tmp;
        Logger::instance()->log("DEBUG: Ingrediens: " + *iter +" Amt: " + tmp);
    }

   return stock;


}
