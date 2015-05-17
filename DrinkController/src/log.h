#ifndef LOG
#define LOG

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <ctime>

using namespace std;


class Logger
{
public:
    static Logger* instance();
    ~Logger();
    string getTime(int);
    void log(string data);

private:
    Logger();            //Singleton
    static Logger* instancePtr_;
    ofstream logfile;
};


#endif // LOG

