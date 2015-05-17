#include "src/admin.h"
#include "src/orderadmin.h"
#include <iostream>
using namespace std;




int main()
{
    cout << "**** FakeGUI Testfile ****" << endl << endl;
    Controller me;
    Logger::instance()->log("Main started");

    Admin AdminCT(&me);
    cout << "[+] Admin created" << endl;
    orderAdmin oAdmin(&me);
    cout << "[+] OrderAdmin created" << endl;

}
