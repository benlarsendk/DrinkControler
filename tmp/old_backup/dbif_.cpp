#include "dbif.h"



using namespace std;

DatabaseIF::DatabaseIF()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/stud/Documents/GUI/secondRunningbar/DrinkControler-master/database/drinksdatabase.db");
    if (!db.open())
        lastError = DB_FAILED_TO_OPEN;
    else
        lastError = NO_ERRORS;
    log.log("DB Opened");
}

DatabaseIF::~DatabaseIF()
{
    boost::mutex::scoped_lock(mtx);
    QSqlDatabase::removeDatabase("QSQLITE");
    log.log("DB closed");
}

int DatabaseIF::getLastError()
{
   return lastError;
}

int DatabaseIF::getDrinksName(vector<string> & drinklist)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;
    if(query.exec("SELECT * FROM Drinks"))
    {
        while(query.next())
        {
            QString temp = query.value(0).toString();
            drinklist.push_back(temp.toStdString());
        }
    }
    else lastError = DB_QUERY_FAIL;

    return lastError;
}

int DatabaseIF::getAddress(string name, vector <int> & addr)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;
    vector<string> nameholder;
    vector<string>::iterator iter;

    if(query.exec("SELECT * FROM Drinks"))
    {
        while(query.next())
        {
            QString temp = query.value(0).toString();
            string tmpString = temp.toStdString();
            if (tmpString == name)
            {
                //Tag fra plads 1,3,5,7,9
                for (int i = 0; i < 8; i++)
                {   i++;
                    QString tmpName = query.value(i).toString();
                    nameholder.push_back(tmpName.toStdString());

                }
            }
         }
    }
    else return DB_QUERY_FAIL;

    if (query.exec("SELECT * FROM Ingredienser"))
    {
        while(query.next())
        {
            for (iter = nameholder.begin(); iter != nameholder.end(); iter++)
            {
                QString temp = query.value(0).toString();
                string tmpString = temp.toStdString();
                //cout << "Indholder af iter: " << *iter << endl;
                if (*iter == tmpString)
                {
                    addr.push_back(query.value(1).toInt());

                }
            }

        }

    }
    return NO_ERRORS;
}

bool DatabaseIF::checkName(int choice, string searchfor)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;

    if (choice == DRINK) // Check på drink
    {
        if(query.exec("SELECT * FROM Drinks"))
        {
            while(query.next())
            {
                QString temp = query.value(0).toString();
                string tmpString = temp.toStdString();
                if (tmpString == searchfor)
                {
                    return true;
                }

             }
        }
    }
    else if (choice == INGREDIENT) // Check på drink
    {
        if(query.exec("SELECT * FROM Ingredienser"))
        {
            while(query.next())
            {
                QString temp = query.value(0).toString();
                string tmpString = temp.toStdString();
                if (tmpString == searchfor)
                {
                    return true;
                }

             }
        }
    }
    else
    {
        lastError = UNDEFINED_PARAMETER;
        return false;
    }
    return false;
}

int DatabaseIF::getIngredientsName(vector <string> & contents)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;
    if(query.exec("SELECT * FROM Ingredienser"))
    {
        while(query.next())
        {
            QString temp = query.value(0).toString();
            contents.push_back(temp.toStdString());
        }
    }
    else lastError = DB_QUERY_FAIL;

    return lastError;

}

int DatabaseIF::createDrink(Drink & inDrink)
{
    boost::mutex::scoped_lock(mtx);
    if (!checkName(DRINK,inDrink.name))
    {
        //Nedenstående burde være generisk
        QString bindQuery = "INSERT INTO Drinks ([Navn],[cont_1_name],[cont_1_amt],[cont_2_name],[cont_2_amt],[cont_3_name],[cont_3_amt],[cont_4_name],[cont_4_amt],[cont_5_name],[cont_5_amt],[path]) VALUES(:name,:cname1,:camt1,:cname2,:camt2,:cname3,:camt3,:cname4,:camt4,:cname5,:camt5,:path)";
        QSqlQuery query;
        query.prepare(bindQuery);

        if (inDrink.name != "NOT_DECLARED"){
            query.bindValue(":name",QString::fromStdString(inDrink.name));
        }

        query.bindValue(":path",QString::fromStdString(inDrink.path));

        for (int i = 0; i < 5; i++){
            ostringstream csname;       // Ostring så vi kan appende i
            ostringstream csamt;        // Same
            csname << ":cname" << i+1;  //Appender
            csamt << ":camt" << i+1;    // Apennder (for recognizion
            string cname(csname.str()); // Laver det om til en string
            string camt(csamt.str());   //String

            if(inDrink.content[i].name != "NOT_DECLARED"){
                query.bindValue(QString::fromStdString(cname),QString::fromStdString((inDrink.content[i].name)));
                query.bindValue(QString::fromStdString(camt),QString::number(inDrink.content[i].amount));
            }
        }
        if (query.exec())
        {
            lastError = NO_ERRORS;
            log.log("Drink: " + inDrink.name + " Has been added to the DB");
            return lastError;
        }
        else
        {
            lastError = DB_QUERY_FAIL;
            return lastError;
        }

    }
    else
    {
        lastError = DB_ENTRY_ALREADY_EXIST;
        return lastError;
    }

}

int DatabaseIF::getDrink(string namesearch, Drink & outDrink)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;
    if(query.exec("SELECT * FROM Drinks"))
    {
        while(query.next())
        {
            QString temp = query.value(0).toString();
            string tmpString = temp.toStdString();
            if (tmpString == namesearch)
            {
                int x = 0;
                outDrink.name = query.value(0).toString().toStdString();
                outDrink.path = query.value(11).toString().toStdString();
                for (int i = 0; i < 5; i++){
                    outDrink.content[i].name = query.value(x+1).toString().toStdString();
                    outDrink.content[i].amount = query.value(x+2).toInt();
                    x += 2;
                }
                lastError = NO_ERRORS;
                return lastError;
            }
            else
            {
                lastError = DB_DRINK_NOT_FOUND;
            }

         }
    }
 return lastError;
}

bool DatabaseIF::checkForUse(string name)
{
    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;

    if(query.exec("SELECT * FROM Drinks"))
    {
        while(query.next())
        {
            for (int i = 0; i < 8; i++)
            {   i++;
                if (query.value(i).toString().toStdString() == name)
                    return true;
            }

         }
    }
    else return DB_QUERY_FAIL;
    return false;


}

bool DatabaseIF::checkContainerInUse(int addr){

    boost::mutex::scoped_lock(mtx);
    QSqlQuery query;

    if(query.exec("SELECT * FROM Ingredienser"))
    {
        while(query.next())
        {
            if (query.value(1).toInt() == addr){
                return true;
            }

         }
        return false;
    }
    return false;

}

int DatabaseIF::changeDrink(Drink & change)
{
    boost::mutex::scoped_lock(mtx);
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
        command = "DELETE FROM Drinks WHERE Navn='" + toDelete + "'";
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
         string command = "DELETE FROM Ingredienser WHERE Navn='" + toDelete + "'";
    }
    else return UNDEFINED_PARAMETER;

    QSqlQuery query;

    boost::mutex::scoped_lock(mtx);
    query.prepare(QString::fromStdString(command));
    if(query.exec())
    {
        lastError = NO_ERRORS;
        if (choice == INGREDIENT)
            log.log("Ingredient: " + toDelete + " has been removed from the DB");
        else if(choice == DRINK)
            log.log("Drink: " + toDelete + " has been removed from the DB");

    }
    else lastError = DB_QUERY_FAIL;
    return lastError;

}

int DatabaseIF::createIngredient(string name, int addr)
{
    if(!checkName(INGREDIENT,name)){
        boost::mutex::scoped_lock(mtx);
        QString bindQuery = "INSERT INTO Ingredienser ([Navn],[Addr]) VALUES(:name,:addr)";
        QSqlQuery query;
        query.prepare(bindQuery);

        query.bindValue(":name",QString::fromStdString(name));
        query.bindValue(":addr",QString::number(addr));

        if (query.exec()){
            lastError = NO_ERRORS;
            log.log("Ingredient " + name + "has been added to the DB");
        }
        else lastError = DB_QUERY_FAIL;
    }
    else lastError = DB_ENTRY_ALREADY_EXIST;

    return lastError;

}

int DatabaseIF::getIngAdress(string getaddr, int & addr)
{

    if (checkName(INGREDIENT,getaddr)){
        boost::mutex::scoped_lock(mtx);
        QSqlQuery query;
        if(query.exec("SELECT * FROM Ingredienser")){
           while(query.next())
           {
                if (query.value(0).toString().toStdString() == getaddr){
                    addr = query.value(1).toInt();
                    lastError = NO_ERRORS;
                }

           }

        }
        else lastError = DB_QUERY_FAIL;
    }
    else lastError = DB_INGREDIENS_NOT_FOUND;
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
    boost::mutex::scoped_lock(mtx);
    QString bindQuery = "INSERT INTO Bestillinger ([Dag],[Måned],[År],[Navn]) VALUES(:day,:month,:year,:name)";
    QSqlQuery query;
    query.prepare(bindQuery);
    string timenow = log.getTime(0);
    string day = timenow.substr(0,2);
    string month = timenow.substr(2,2);
    string year = timenow.substr(4,4);

    query.bindValue(":day",QString::fromStdString(day));
    query.bindValue(":month",QString::fromStdString(month));
    query.bindValue(":year",QString::fromStdString(year));
    query.bindValue(":name",QString::fromStdString(name));

    if (query.exec()){
        lastError = NO_ERRORS;
        log.log("Order: " + name + "has been saved to the DB");
    }
    else lastError = DB_QUERY_FAIL;

    return lastError;
}









