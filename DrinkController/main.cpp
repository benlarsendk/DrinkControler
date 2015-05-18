//#include "src/admin.h"
#include "src/orderadmin.h"
#include <iostream>
#include <boost/thread.hpp>
using namespace std;


void startAdmin(Controller * me, DatabaseIF* ddb)
{
    cout << "[+] Starting thread for Admin-control" << endl;
    //Admin AdminCT(me,ddb);

}


int main()
{
    DatabaseIF* ddb = new DatabaseIF;

    cout << "**** FakeGUI Testfile ****" << endl << endl;
    Controller me;


  //  boost::thread adminT(startAdmin,&me,ddb);

    cout << "[+] Creating order-admin" << endl;
    orderAdmin * oa = new orderAdmin(&me,ddb);
    vector<string> drinks = oa->getDrinksName();

    for (vector<string>::iterator iter = drinks.begin(); iter != drinks.end(); iter++){
        cout << *iter << endl;
    }



/*

    //oa->getDrinksName();

    map<string,string> stock;
    stock = oa->checkStock();

    for (map<string,string>::iterator iter = stock.begin(); iter != stock.end(); iter++){
        cout << "Name: " << iter->first << "\nStock: " << iter->second << endl;
    }
*/
    oa->worker->join();


    //adminT.join();*/

}
