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

int execute(char* cmd) {

    FILE *in;
    char buff[2048];

    if (!(in = popen(cmd, "r"))) {
        return 1;
    }
    while (fgets(buff, sizeof(buff), in) != NULL) {
        cout << buff;
    }

    pclose(in);
    return 0;
}


int main()
{
    FILE *in;

    if(!(in = popen("./start.sh", "r"))){
        cout << "[-] Couldn't get spidevice. Exiting" << endl;
        pclose(in);
        exit(1);
    }

    pclose(in);

    DatabaseIF* ddb = new DatabaseIF;

    cout << "**** FakeGUI Testfile ****" << endl << endl;
    Controller me;


    boost::thread adminT(startAdmin,&me,ddb);

    orderAdmin * oa = new orderAdmin(&me,ddb);

    vector<string> drinks;

   // drinks.push_back("Vodka_og_Redbull");

    //oa->orderDrinks(drinks);

    oa->worker->join();
    adminT.join();


}
