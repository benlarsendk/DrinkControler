#include "dbif.h"



using namespace std;

DatabaseIF::DatabaseIF()
{
    Logger::instance()->log("DEBUG: DBIF CTOR loaded.");
  //    char* dbname = "/home/root/drinksdatabase.db";
    char* dbname = "/home/deblab/Dropbox/IHA/3SEM/PRJ3/Controllers_THIS_IS_WORKING_MED_DBIF_OSV/admin/DrinkController/database/drinksdatabase.db";
    if(sqlite3_open(dbname,&db) == SQLITE_OK){
        lastError = NO_ERRORS;
        //cout << "Database loaded." << endl;
        Logger::instance()->log("Database loaded.");
    }
    else{
        lastError = DB_FAILED_TO_OPEN;
        Logger::instance()->log("Database failed to open");
    }
}

DatabaseIF::~DatabaseIF()
{
    Logger::instance()->log("Database closed.");
    sqlite3_close(db);
}



vector<vector<string> > DatabaseIF::query(string query_)
{
    boost::mutex::scoped_lock(mtx);
    char query[1024];
    strcpy(query, query_.c_str());

    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare(db, query, -1, &statement, 0) ==  SQLITE_OK){
        Logger::instance()->log("Query to DB: "+query_+" executed.");
        lastError = NO_ERRORS;

        int cols = sqlite3_column_count(statement);
        int result = 0;

        for(;;)
        {
                result = sqlite3_step(statement);
                if (result == SQLITE_ROW)
                {
                    vector<string> values;
                    for(int i = 0; i < cols; i++)
                    {

                    string val;
                    char* ptr = (char*)sqlite3_column_text(statement,i);

                    if(ptr)
                    {
                        val = ptr;
                    }
                    else val = "";

                    values.push_back(val);
                }
                results.push_back(values);

                }
                else break;
        }
        sqlite3_finalize(statement);
    }
    else{
        lastError = DB_QUERY_FAIL;
        Logger::instance()->log("Query failed.");
    }
    return results;


}



int DatabaseIF::getLastError()
{
   return lastError;
}


int DatabaseIF::getDrinksName(vector<string> & drinklist)
{
    vector<vector<string> > results;
    results = query("SELECT * FROM Drinks");

    for(vector<vector<string> >::iterator iter = results.begin(); iter != results.end(); iter++){
        vector<string> row = *iter;
        drinklist.push_back(row.at(0));
        drinklist.push_back(row.at(11));

        }

    return lastError;
}

int DatabaseIF::getAddress(string name, vector <int> & addr)
{
    vector<vector<string> > results = query("SELECT * FROM Drinks");
    vector<vector<string> > ings = query("SELECT * FROM Ingredienser");

    vector<string> nameholder;

    for(vector<vector<string> >::iterator iter = results.begin(); iter != results.end(); iter++){
        vector<string> row = *iter;
        if(row.at(0) == name)
        {
            for (int i = 0; i < 8; i++){
                nameholder.push_back(row.at(i));
            }
        }
    }

    for (vector<vector<string> >::iterator it = ings.begin(); it != ings.end(); it++){
        vector<string>row = *it;

        for(vector<string>::iterator xter = nameholder.begin(); xter != nameholder.end(); xter++){
            if(row.at(0) == *xter){
                string tmp = row.at(1);
                addr.push_back(atoi(tmp.c_str()));
            }
        }
    }

    return lastError;
}


bool DatabaseIF::checkName(int choice, string searchfor)
{


    vector<vector<string> > result;

    if (choice == DRINK) // Check på drink
    {
        result = query("SELECT * FROM Drinks");
    }
    else if (choice == INGREDIENT)
    {
        result = query("SELECT * FROM Ingredienser");
    }
    else{
        lastError = UNDEFINED_PARAMETER;
        return false;
    }


    for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
        vector<string>row = *iter;
        if(row.at(0) == searchfor)
            return true;
        }

    return false;

}

int DatabaseIF::getIngredientsName(vector <string> & contents)
{

    vector<vector<string> > result = query("SELECT * FROM Ingredienser");

    for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
        vector<string> row = *iter;
        contents.push_back(row.at(0));
    }
    return lastError;
}

int DatabaseIF::createDrink(Drink & inDrink)
{

    if (!checkName(DRINK,inDrink.name))
    {

        string qu = "INSERT INTO Drinks VALUES(";
        qu.append("'");
        qu.append(inDrink.name);
        qu.append("'");
        qu.append(", ");
        for (int i = 0; i < 5; i++){
            qu.append("'");
            qu.append(inDrink.content[i].name);
            qu.append("'");
            qu.append(", ");
            int tmpamt = inDrink.content[i].amount;
            stringstream ss;
            ss << tmpamt;
            string tmpamt_s = string(ss.str());
            qu.append(tmpamt_s);
            qu.append(", ");
        }
        qu.append("'");
        qu.append(inDrink.path);
        qu.append("'");
        qu.append(");");

        query(qu);
    }
    else{
        lastError = DB_ENTRY_ALREADY_EXIST;
        return lastError;
    }
    return lastError;
}

int DatabaseIF::getDrink(string namesearch, Drink & outDrink)
{


    vector<vector<string> > result = query("SELECT * FROM Drinks");

    for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
        vector<string> row = *iter;

        if(row.at(0) == namesearch){
            int x = 0;
            outDrink.name = row.at(0);
            for (int i = 0; i < 5; i++){
                outDrink.content[i].name = row.at(x+1);
                string amt = row.at(x+2);
                outDrink.content[i].amount = atoi(amt.c_str());
                x+=2;
            }
            outDrink.path = row.at(11);
            lastError = NO_ERRORS;
            return lastError;
        }

    }
    lastError = DB_DRINK_NOT_FOUND;
    return lastError;

}

bool DatabaseIF::checkForUse(string name)
{

    vector<vector<string> > result = query("SELECT * FROM Drinks");

    for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
        vector<string> row = *iter;

        for(vector<string>::iterator it = row.begin(); it != row.end(); it++){
            if(*it == name){
                return true;
            }
        }

    }
    return false;
}

bool DatabaseIF::checkContainerInUse(int addr){



    vector<vector<string> > result = query("SELECT * FROM Ingredienser");
    for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
           vector<string> row = *iter;
           string tmps = row.at(1);
           int cont = atoi(tmps.c_str());
           if (cont == addr){
               return true;
           }

    }
    return false;
}

int DatabaseIF::changeDrink(Drink & change)
{

   remove(change.name,DRINK);
   createDrink(change);
   return lastError;
}

int DatabaseIF::remove(string toDelete, int choice)
{

    string command;
    if (choice == DRINK){
        if (!checkName(DRINK,toDelete)){
            lastError = DB_DRINK_NOT_FOUND;
            return lastError;
        }
        command = "DELETE FROM Drinks WHERE Navn='" + toDelete + "'" + ";";
    }
    else if (choice == INGREDIENT){
        if (!checkName(INGREDIENT,toDelete)){
            lastError = DB_INGREDIENS_NOT_FOUND;
            return lastError;
        }
        if (checkForUse(toDelete)){
            lastError = DB_INGREDIENS_IN_USE;
            return lastError;
        }
         command = "DELETE FROM Ingredienser WHERE Navn='" + toDelete + "'" + ";";
    }
    else return UNDEFINED_PARAMETER;

    query(command);
    return lastError;
}

int DatabaseIF::createIngredient(string name, int addr)
{
    if(!checkName(INGREDIENT,name)){
        string cmd = "INSERT INTO Ingredienser VALUES(";
        cmd.append("'");
        cmd.append(name);
        cmd.append("', ");
        stringstream ss;
        ss << addr;
        cmd.append(ss.str());
        cmd.append(");");

        query(cmd);
        return lastError;
    }
    else{
        lastError = DB_ENTRY_ALREADY_EXIST;
        return lastError;
    }
}

int DatabaseIF::getIngAdress(string getaddr, int & addr)
{
    if (checkName(INGREDIENT,getaddr)){

        vector<vector<string> > result = query("SELECT * FROM Ingredienser");
        for(vector<vector<string> >::iterator iter = result.begin(); iter != result.end(); iter++){
               vector<string> row = *iter;
               if(row.at(0) == getaddr){
                   string tmprow = row.at(1);
                   addr = atoi(tmprow.c_str());
               }
        }
    }

    return lastError;
}


int DatabaseIF::changeIngrediensAddr(string ing, int newadd)
{
    remove(ing,INGREDIENT);
    createIngredient(ing,newadd);
    return lastError;
}

int DatabaseIF::saveOrder(string name)
{
    string cmd = "INSERT INTO Bestillinger VALUES(";
    string timenow = Logger::instance()->getTime(0);
    cmd.append(timenow.substr(0,2));
    cmd.append(", ");
    cmd.append(timenow.substr(2,2));
    cmd.append(", ");
    cmd.append(timenow.substr(4,4));
    cmd.append(", '");
    cmd.append(name);
    cmd.append("');");

    query(cmd);

    return lastError;
}









