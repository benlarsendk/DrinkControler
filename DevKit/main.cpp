#include "src/admin.h"
#include "src/orderadmin.h"
#include <iostream>
#include <boost/thread.hpp>
using namespace std;


void startAdmin(Controller * me, DatabaseIF* ddb)
{
    cout << "[+] Starting thread for Admin-control" << endl;
    Admin AdminCT(me,ddb);

}


int main()
{
    DatabaseIF* ddb = new DatabaseIF;

    cout << "**** FakeGUI Testfile ****" << endl << endl;
    Controller me;


    boost::thread adminT(startAdmin,&me,ddb);

    orderAdmin * oa = new orderAdmin(&me,ddb);

    vector<string> drinks;

    drinks.push_back("Vodka_og_Redbull");

    oa->orderDrinks(drinks);

    oa->worker->join();
    adminT.join();


}
