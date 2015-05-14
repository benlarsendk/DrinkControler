#include <src/adminclient.h>
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

    client->send(tosend);
}

void AdminClient::clean()
{
    string tosend;
    stringstream out;
    out << CHANGEDRINK;
    tosend = out.str();
    client->send(tosend);
    string in = client->recieve();
    GUINF->print(in);
}

void AdminClient::clean_water()
{
    string tosend = to_string(CLEAN_WATER);
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
    client->send(tosend);
}

int AdminClient::getIngredientAddress(string ingredient)
{
    string tosend = to_string(GETINGREDIENTADDR);
    tosend.append(":");
    tosend.append(ingredient);
    client->send(tosend);
    string in = client->recieve();
    return atoi(in.c_str());
}

bool AdminClient::checkNameDrink(string namecheck)
{
    string tosend = to_string(CHECKNAMEDRINK);
    tosend.append(":");
    tosend.append(namecheck);
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
    for (int i = 0; i < 5; i++){
     tosend.append(newDrink.content[i].name);
     tosend.append(":");
     tosend.append(to_string(newDrink.content[i].amount));
     tosend.append(":");
    }
    tosend.append(newDrink.path);

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

    //første første, anden anden

    map<string,string> stock; // Navn, mængde
    string tmp;
    vector<string> name;
    vector<string> amt;

    // Tager navne
    for (unsigned int i = 0; i < in.size(); i++){
        if(in.at(i) == ':'){
            name.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += in.at(i);
        }

        i++;
    }

    for (unsigned int i = 0; i < in.size(); i++){
        if(in.at(i) == ':'){
            amt.push_back(tmp);
            tmp = "";
        }
        else tmp += in.at(i);

        i++;
    }


    for (vector<string>::iterator iter = name.begin(); iter != name.end(); iter++){

        stock[*iter] = "0000"; // fixme
    }

    return stock;
}

Drink AdminClient::getDrink(string name)
{

    string tosend = to_string(GETDRINK);
    tosend.append(":");
    tosend.append(name);
    client->send(tosend);
    string in = client->recieve();

    Drink newDrink;
    newDrink.name = in.at(1);
    for (int i = 0; i < 5; i++){
        newDrink.content[i].name = in.at(i+2);
        string amt;
        amt += in.at(i+3);
        newDrink.content[i].amount = atoi(amt.c_str());
    }
    newDrink.path = in.at(12);

    return newDrink;}
