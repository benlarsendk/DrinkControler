#include "adminclient.h"
#include <stdlib.h>



AdminClient::AdminClient(Controller* ctrl)
{
    client = new Client;
    GUINF = ctrl;
}

AdminClient::~AdminClient()
{
    delete client;
}


void AdminClient::changeDrink(Drink drinktoedit)
{
    string tosend;
    stringstream out;
    out << CHANGEDRINK;
    tosend = out.str();

    tosend.append(":");
    tosend.append(drinktoedit.name);
    tosend.append(":");
    for (int i = 0; i < 5; i++){
     tosend.append(drinktoedit.content[i].name);
     tosend.append(":");
     int tempint = drinktoedit.content[i].amount;
     string tmp = to_string(tempint);
     tosend.append(tmp);
     tosend.append(":");
    }
    tosend.append(drinktoedit.path);
    tosend.append(":");

    client->send(tosend);
}

void AdminClient::clean()
{
    string tosend = to_string(CLEAN);
    tosend.append(":");
    client->send(tosend);
    string in = client->recieve();
    GUINF->print(in);
}

void AdminClient::clean_water()
{
    string tosend = to_string(CLEAN_WATER);
    tosend.append(":");
    client->send(tosend);
    string in = client->recieve();
    GUINF->print(in);

}

void AdminClient::getIngredientsName(vector <string> & currentIngredients){

    string tosend = to_string(GETINGREDIENTSNAME);
    client->send(tosend);
    string in = client->recieve();
    string tmp;

    for (unsigned int i = 0; i < in.size(); i++){
        if(in.at(i) == ':'){
            currentIngredients.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += in.at(i);
        }
    }
}

bool AdminClient::deleteDrink(string todelte)
{
    string tosend = to_string(DELETEDRINK);
    tosend.append(":");
    tosend.append(todelte);
    tosend.append(":");
    client->send(tosend);

    if(client->recieve() == "TRUE"){
        return true;
    }
    else return false;
}



bool AdminClient::checkNameIngredient(string name)
{
    string tosend = to_string(CHECKNAMEINGREDIENT);
    tosend.append(":");
    tosend.append(name);
    tosend.append(":");
    client->send(tosend);


    if (client->recieve() == "TRUE"){
        return true;
    }
    else return false;
}
bool AdminClient::checkContainer(int addr)
{
    string tosend = to_string(CHECKCONTAINER);
    tosend.append(":");
    tosend.append(to_string(addr));
    client->send(tosend);
    tosend.append(":");

    if (client->recieve() == "TRUE"){
        return true;
    }
    else return false;

}

bool AdminClient::changeIngredientAddr(string name,int newAddr)
{
    string tosend = to_string(CHANGEINGREDIENTADDR);
    tosend.append(":");
    tosend.append(name);
    tosend.append(":");
    tosend.append(to_string(newAddr));
    tosend.append(":");
    client->send(tosend);


    if (client->recieve() == "TRUE"){
        return true;
    }
    else return false;
}

bool AdminClient::deleteIngredient(string todelte)
{
    string tosend = to_string(DELETEINGREDIENT);
    tosend.append(":");
    tosend.append(todelte);
    tosend.append(":");
    client->send(tosend);

    if (client->recieve() == "TRUE"){
        return true;
    }
    else return false;
}
void AdminClient::createIngredient(string name, int addr)
{
    string tosend = to_string(CREATEINGREDIENT);
    tosend.append(":");
    tosend.append(name);
    tosend.append(":");
    tosend.append(to_string(addr));
    tosend.append(":");
    client->send(tosend);
}

int AdminClient::getIngredientAddress(string ingredient)
{
    string tosend = to_string(GETINGREDIENTADDR);
    tosend.append(":");
    tosend.append(ingredient);
    tosend.append(":");
    client->send(tosend);
    string in = client->recieve();
    return atoi(in.c_str());
}

bool AdminClient::checkNameDrink(string namecheck)
{
    string tosend = to_string(CHECKNAMEDRINK);
    tosend.append(":");
    tosend.append(namecheck);
    tosend.append(":");
    client->send(tosend);

    if(client->recieve() == "TRUE"){
        return true;
    }
    else return false;
}

void AdminClient::createDrink(Drink newDrink)
{
    string tosend = to_string(CREATEDRINK);
    tosend.append(":");
    tosend.append(newDrink.name);
    tosend.append(":");

    for (unsigned int i = 0; i < 5; i++){
        tosend.append(newDrink.content[i].name);
        tosend.append(":");
        tosend.append(to_string(newDrink.content[i].amount));
        tosend.append(":");
    }
    tosend.append(newDrink.path);
    tosend.append(":");
    client->send(tosend);
}

std::string AdminClient::getErrorPT(int err)
{
    string tosend = to_string(GETERROR);
    tosend.append(":");
    tosend.append(to_string(err));
    client->send(tosend);
    return client->recieve();
}

std::vector<string> AdminClient::getDrinksName()
{
    string tosend = to_string(GETDRINKSNAME);
    client->send(tosend);
    vector<string> returnz;

    string in = client->recieve();
    string tmp;

    for (unsigned int i = 0; i < in.size(); i++){
        if(in.at(i) == ':'){
            returnz.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += in.at(i);
        }
    }

    return returnz;

}
std::map<string,string> AdminClient::checkStock()
{
    string tosend = to_string(CHECKSTOCK);
    client->send(tosend);

    string in = client->recieve();

    //string in = ":Appelsinjuice:10:Cola:15:Grenadine sirup:15:";
    map<string,string> stock; // Navn, mængde
    string tmp;
    vector<string> sheit;

    for (unsigned int i = 0; i < in.size(); i++){
        if(in[i] == ':'){
            sheit.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += in[i];
        }
    }


    for (vector<string>::iterator iter = sheit.begin(); iter != sheit.end(); iter++){
        string tmp = *iter;
        iter++;
        stock[tmp] = *iter;
    }

    return stock;
}


std::map<string,string> AdminClient::getTemp()
{
    string tosend = to_string(GETTEMP);
    client->send(tosend);

    string in = client->recieve();

    map<string,string> temp; // Navn, temp
    string tmp;
    vector<string> sheit;

    for (unsigned int i = 0; i < in.size(); i++){
        if(in[i] == ':'){
            sheit.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += in[i];
        }
    }


    for (vector<string>::iterator iter = sheit.begin(); iter != sheit.end(); iter++){
        string tmp = *iter;
        iter++;
        temp[tmp] = *iter;
    }

    return temp;
}

void AdminClient::debug()
{
    client->send("99");
    string in = client->recieve();

    std::cout << in << std::endl;
}

void AdminClient::listen()
{
    for(;;){
        string in = client->recieve();
        std::cout << in << std::endl;
    }
}

Drink AdminClient::getDrink(string name)
{

    string tosend = to_string(GETDRINK);
    tosend.append(":");
    tosend.append(name);
    tosend.append(":");
    client->send(tosend);
    string in = client->recieve();

    string* data = new string[100];

    int count = 0;
    for (int i = 0; i < 100; i++){
        data[i] = "*";
    }
    string tmp ="";

    for (unsigned int i = 0; i < in.length(); i++){
        tmp += in[i];
        if (in[i] == ':'){
            string push = tmp.substr(0, tmp.size()-1);
            data[count] = push;
            count++;
            tmp = "";
            push ="";
        }
    }

    Drink newDrink;
    string amt;

    newDrink.name = data[0];
    newDrink.content[0].name = data[1];
    amt = data[2];
    newDrink.content[0].amount = atoi(amt.c_str());

    newDrink.content[1].name = data[3];
    amt = data[4];
    newDrink.content[1].amount = atoi(amt.c_str());

    newDrink.content[2].name = data[5];
    amt = data[6];
    newDrink.content[2].amount = atoi(amt.c_str());

    newDrink.content[3].name = data[7];
    amt = data[8];
    newDrink.content[3].amount = atoi(amt.c_str());

    newDrink.content[4].name = data[9];
    amt = data[10];
    newDrink.content[4].amount = atoi(amt.c_str());
    amt = data[11];
    newDrink.content[4].amount = atoi(amt.c_str());

    newDrink.path = data[12];


    return newDrink;
}
