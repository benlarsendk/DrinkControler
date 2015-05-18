#include "admin.h"


Admin::Admin(Controller *controller, DatabaseIF* dbI)
{
   db = dbI;
   GUINF = controller;
   server = new Server(this);
   server->start();
}


Admin::~Admin(){

    delete server;
}

bool Admin::checkNameDrink(string namecheck)
{
    if(db->checkName(DRINK,namecheck)){
        return false;
    }
    else{
       return true;
    }

}

void Admin::getIngredientsName(vector <string> & currentIngredients)
{
    if(db->getIngredientsName(currentIngredients)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        currentIngredients.push_back("DB ERROR: " + getErrorPT(db->getLastError()));
        return;
    }
}


bool Admin::createDrink(Drink newDrink)
{
    if(db->createDrink(newDrink)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return false;
    }
    else return true;
}

vector<string> Admin::getDrinksName()
{
    vector<string> drinks;
    db->getDrinksName(drinks);
    if(db->getLastError()!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return drinks;
    }
    else{
        return drinks;
    }
}

map<string,string> Admin::checkStock()
{
    vector<string> ings;

    if(db->getIngredientsName(ings)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
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

Drink Admin::getDrink(string name)
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

bool Admin::changeDrink(Drink drinktoedit)
{
    if(db->changeDrink(drinktoedit)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return false;
    }
    else{
        return true;
    }
}


bool Admin::deleteDrink(string todelete)
{
       if(db->remove(todelete,DRINK)!=0){
           Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
           return false;
       }
       else{
          Logger::instance()->log("The entry "+ todelete + " has been deleted.");
          return true;
       }
}


bool Admin::checkNameIngredient(string name)
{
    return db->checkName(INGREDIENT,name);
}

bool Admin::checkContainer(int addr)
{
    if(db->checkContainerInUse(addr)){ //return true hvis findes
        return false;
    }
    else return true;
}

bool Admin::createIngredient(string name, int addr)
{
    if(db->createIngredient(name,addr)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return false;
    }
    else{
        return true;
    }
}

int Admin::getIngredientAddress(string ingredient)
{
    int addr;
    if(db->getIngAdress(ingredient,addr)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return addr;
    }
    else{
        return addr;
    }
}


bool Admin::changeIngredientAddr(string name,int newAddr)
{
    if(db->changeIngrediensAddr(name,newAddr)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return false;
    }
    else{
        return true;
    }
}

bool Admin::deleteIngredient(string todelte)
{
    if(db->checkForUse(todelte)){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        return false;
    }
    else{
        if(db->remove(todelte,INGREDIENT)!=0){
            Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
            return false;
        }
        else{
            return true;
        }
    }
}



void Admin::clean()
{
	int fd = open("/dev/spidev", O_RDWR);	
    u_int8_t clean = 0x03;
    char buff[8];


    write(fd,&clean,8);

    while(read(fd,buff,8) == 0){
        sleep(1);
	}
}

void Admin::clean_water(){
    u_int8_t clean = 0x03;
    int fd = open("/dev/spidev", O_RDWR);
    char buff2[8];

    write(fd,&clean,8);

    while(read(fd,buff2,8) == 0){
        sleep(1);
    }
}

void Admin::decode(string encoded, vector <string> & decoded){

    string tmp;
    for (unsigned int i = 3; i < encoded.length(); i++){

        if (encoded[i] == ':'){
            decoded.push_back(tmp);
            tmp = "";
        }
        else tmp = tmp + encoded[i];
    }

}

void Admin::parser(char * input, int mySock){
    if(input != NULL){
        string data(input);
        string cmd;
        if (data[1] == ':'){
            cmd = data[0];
        }
        else cmd = data.substr(0,2);

        int id = atoi(cmd.c_str()); // få ID'et

        vector<string> newData;
        decode(data, newData);
        string tosend;

        tosend = "";
        switch(id){

        case CHECKNAMEDRINK:
            {
                if(checkNameDrink(newData.at(0)) == true){
                server->send("TRUE",mySock);
            }
                else{
                    server->send("FALSE",mySock);
                    }
                break;
        }
        case CHECKSTOCK:
            {
                map<string,string> ings = checkStock();
                for (map<string,string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
                    tosend.append(iter->first);
                    tosend += ":";
                    tosend.append(iter->second);
                    tosend += ":";
                }
                server->send(tosend,mySock);
                break;

            }
        case GETINGREDIENTSNAME:
            {
                vector<string> current;
                getIngredientsName(current);

                for(vector<string>::iterator iter = current.begin(); iter != current.end(); iter++){
                    tosend += *iter;
                    tosend += ":";
                }
                server->send(tosend,mySock);
                break;
            }
        case CREATEDRINK:
            {

                Drink newDrink;
                newDrink.name = newData.at(0);
                for (int i = 0; i < 5; i++){
                    newDrink.content[i].name = newData.at(i+1);
                    string amt = newData.at(i+2);
                    newDrink.content[i].amount = atoi(amt.c_str());
                }
               // newDrink.path = newData.at(12);
                if (createDrink(newDrink))
                {
                   server->send("TRUE",mySock);
                }
                else server->send("FALSE",mySock);

                break;
            }

        case GETDRINKSNAME:
            {
            /*
                vector<string> drinks = getDrinksName();

                for (vector<string>::iterator iter = drinks.begin(); iter != drinks.end(); iter++){
                    tosend += *iter;
                    tosend += ":";
                }
                server->send(tosend,mySock);
             */    break;
            }

        case GETDRINK:
            {
                Drink local = getDrink(newData.at(1));

                tosend += local.name;
                tosend += ":";
                for (int i = 0; i < 5; i++){
                    tosend+=local.content[i].name;
                    tosend+=":";
                    tosend+=local.content[i].amount;
                    tosend+=":";
                }
                tosend += local.path;
                tosend += ":";
                server->send(tosend,mySock);
                break;
            }

        case CHANGEDRINK:
                {
                    Drink newDrink;
                    newDrink.name = newData.at(1);
                    for (int i = 0; i < 5; i++){
                        newDrink.content[i].name = newData.at(i+2);
                        string amt = newData.at(i+3);
                        newDrink.content[i].amount = atoi(amt.c_str());
                    }
                    newDrink.path = newData.at(12);
                    if (createDrink(newDrink) == true)
                    {
                        //server->send("TRUE",mySock);

                    }
                    //else server->send("FALSE",mySock);
                    break;
                }

        case DELETEDRINK:
        {
            if (deleteDrink(newData.at(1))){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);
            break;
        }


        case CHECKNAMEINGREDIENT:
        {
            if (checkNameIngredient(newData.at(1)))
            {
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);

            break;
        }

        case CHECKCONTAINER:
        {
            string par = newData.at(1);

            if (checkContainer(atoi(par.c_str()))){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);

            break;
        }
            //

        case CREATEINGREDIENT: // string name int addr
        {
            string name = newData.at(1);
            string addr = newData.at(2);
            int rAddr = atoi(addr.c_str());

            if(createIngredient(name,rAddr)){
                //return true;
            }
            break;
        }
        case GETINGREDIENTADDR:    
        {
            int addr = getIngredientAddress(newData.at(1));
            string tmp;
            tmp += addr;
           // string tmp = std::to_string(addr);
            server->send(tmp,mySock);
        }
            //
            break;
        case CHANGEINGREDIENTADDR:
        {
            string name = newData.at(1);
            string addr = newData.at(2);
            int rAddr = atoi(addr.c_str());
            if(changeIngredientAddr(name,rAddr)){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);
            break;
        }

        case DELETEINGREDIENT:
        {
            if (deleteIngredient(newData.at(1))){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);
            break;
        }

        case CLEAN:
        {
            clean();
            server->send("WATER",mySock);
            break;
        }
        case CLEAN_WATER:
        {

            clean_water();
            server->send("ADD_NORMAL",mySock);
        }

        case GETERROR:
        {
            string tmp = newData.at(1);
            server->send(getErrorPT(atoi(tmp.c_str())),mySock);

        }
        case 99:
        {
            string tmp = "Debugging12345";
            server->send(tmp,mySock);
            break;
        }
        default:
            server->send("WHATTHEFUCK!?!?!",mySock);
            break;
        }


        }
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
