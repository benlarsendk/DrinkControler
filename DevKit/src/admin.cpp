#include "admin.h"
#include "sstream"

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
        return true;
    }
    else{
       return false;
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
    u_int8_t cmd[] = "2";
    write(fd,cmd,8); // stock state
    sleep(2);


    for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
        char tmpBuf[8];
        read(fd,tmpBuf,8);
        string tmp(tmpBuf);
        int smallvalue = atoi(tmp.c_str());
        int realval = 4*smallvalue;

        stringstream ss;
        ss << realval;
        string stringval = ss.str();

        stock[*iter] = stringval;
        Logger::instance()->log("DEBUG: Ingrediens: " + *iter +" Amt: " + tmp);
    }

   return stock;

}

map<string,string> Admin::getTemp()
{
    vector<string> ings;

    if(db->getIngredientsName(ings)!=0){
        Logger::instance()->log("DB ERROR: " + getErrorPT(db->getLastError()));
        map<string,string> error;
        return error;
    }


    map<string,string> temp; // Navn, mængde

    int fd = open("/dev/spidev", O_RDWR);
    u_int8_t cmd[] = "4";
    write(fd,cmd,8); // temp state
    sleep(5);


    for (vector<string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
        char tmpBuf[8];
        read(fd,tmpBuf,8);
        string tmp(tmpBuf);

        temp[*iter] = tmp;
        Logger::instance()->log("DEBUG: Ingrediens: " + *iter +" temp: " + tmp);
    }

   return temp;

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
    if(db->checkContainerInUse(addr)){
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

    if (fd < 0){
        Logger::instance()->log("SPI Device failed to open");
    }
    else
        Logger::instance()->log("SPI Device opened");


    int n;
    u_int8_t clean[] = "3";
    char buff[8];


    n = write(fd,clean,8);
    sleep(1);
    stringstream ss;
    ss << n;
    string str = ss.str();

    if(n > 0){ // success on write
        Logger::instance()->log("Write to PSoC: Clean --> N = " + str);
        while(read(fd,buff,8) < 0){ // wait for read
            Logger::instance()->log("Waiting for PSoC clean");

        }
    }
    else Logger::instance()->log("Write to SPIDEV error");

}

void Admin::clean_water(){
    u_int8_t clean[] = "3";
    int fd = open("/dev/spidev", O_RDWR);
    int n;
    char buff2[8];

    n = write(fd,clean,8);
    stringstream ss;
    ss << n;
    string str = ss.str();


    if (n > 0){
        Logger::instance()->log("Write to PSoC: Clean Water --> N = " + str);
        while(read(fd,buff2,8) < 0){
            Logger::instance()->log("Waiting for PSoC clean #2");
            sleep(1);
        }

    }
    else Logger::instance()->log("Write to SPIDEV error");
}

string* Admin::decode(string encoded){
    Logger::instance()->log("Decoder started");

    string* data = new string[100];

    int count = 0;

    for (int i = 0; i < 100; i++){
        data[i] = "*";
    }


    string tmp ="";

    for (unsigned int i = 0; i < encoded.length(); i++){
        tmp += encoded[i];
        if (encoded[i] == ':'){
            string push = tmp.substr(0, tmp.size()-1);
            data[count] = push;
            count++;
            tmp = "";
            push ="";
        }
    }
    return data;


}

void Admin::parser(char * input, int mySock){
    if(input != NULL){

        string data(input);
        Logger::instance()->log("Parser recieved: " + data);

        string cmd;

        for (int i = 0; i < data.size(); i++){
            if(data[i] == ':'){
                break;
            }
            else cmd += data[i];
        }
        Logger::instance()->log("Command found: " + cmd);
        int id = atoi(cmd.c_str()); // få ID'et



        string* newData = decode(data);
        string tosend ="";

        switch(id){

        case CHECKNAMEDRINK:
            {
                Logger::instance()->log("Entered the case. checking on: " +newData[1]);
                if(checkNameDrink(newData[1]) == true){
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
                newDrink.name = newData[1];
                for (int i = 0; i < 5; i++){
                    newDrink.content[i].name = newData[i+2];
                    string amt = newData[i+3];
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

                vector<string> drinks = getDrinksName();

                for (vector<string>::iterator iter = drinks.begin(); iter != drinks.end(); iter++){
                    tosend += *iter;
                    tosend += ":";
                }
                server->send(tosend,mySock);
                break;
            }

        case GETDRINK:
            {
                Drink local = getDrink(newData[1]);

                tosend += local.name;
                tosend += ":";
                for (int i = 0; i < 5; i++){
                    tosend+=local.content[i].name;
                    tosend+=":";
                    stringstream ss;
                    ss << local.content[i].amount;
                    string str = ss.str();
                    tosend+=ss.str();
                    tosend+=":";
                }
                tosend += local.path;
                Logger::instance()->log("Pushing path " + local.path);
                tosend += ":";
                Logger::instance()->log("gd sending: " + tosend);
                server->send(tosend,mySock);
                break;
            }

        case CHANGEDRINK:
                {
                    Drink newDrink;
                    newDrink.name = newData[1];
                    for (int i = 0; i < 5; i++){
                        newDrink.content[i].name = newData[i+1];
                        string amt = newData[i+2];
                        newDrink.content[i].amount = atoi(amt.c_str());
                    }
                    newDrink.path = newData[11];
                    if (changeDrink(newDrink) == true)
                    {
                        //server->send("TRUE",mySock);

                    }
                    //else server->send("FALSE",mySock);
                    break;
                }

        case DELETEDRINK:
        {
            if (deleteDrink(newData[1])){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);
            break;
        }


        case CHECKNAMEINGREDIENT:
        {
            if (checkNameIngredient(newData[1]))
            {
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);

            break;
        }

        case CHECKCONTAINER:
        {
            string par = newData[1];

            if (checkContainer(atoi(par.c_str()))){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);

            break;
        }


        case CREATEINGREDIENT: // string name int addr
        {
            string name = newData[1];
            string addr = newData[2];
            int rAddr = atoi(addr.c_str());

            if(createIngredient(name,rAddr)){
                //return true;
            }
            break;
        }
        case GETINGREDIENTADDR:    
        {
            int addr = getIngredientAddress(newData[1]);
            stringstream ss;
            ss << addr;
            string tmp = ss.str();
            server->send(tmp,mySock);
            break;
        }


        case CHANGEINGREDIENTADDR:
        {
            string name = newData[1];
            string addr = newData[2];
            Logger::instance()->log("INGNAME: : " + newData[1]);
            Logger::instance()->log("INGADDR: : " + newData[2]);


            int rAddr = atoi(addr.c_str());
            if(changeIngredientAddr(name,rAddr)){
                server->send("TRUE",mySock);
            }
            else server->send("FALSE",mySock);
            break;
        }

        case DELETEINGREDIENT:
        {
            if (deleteIngredient(newData[1])){
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
        case GETTEMP:
        {
            map<string,string> ings = getTemp();
            for (map<string,string>::iterator iter = ings.begin(); iter != ings.end(); iter++){
                tosend.append(iter->first);
                tosend += ":";
                tosend.append(iter->second);
                tosend += ":";
            }
            server->send(tosend,mySock);
            break;

        }
/*
        case GETERROR:
        {
            string tmp = newData.at(1);
            server->send(getErrorPT(atoi(tmp.c_str())),mySock);
            }

        */
        default:
            server->send("WHATTHEFUCK!?!?!",mySock);
            break;
        }
        delete[] newData;


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
