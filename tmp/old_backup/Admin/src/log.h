#ifndef LOG
#define LOG

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <ctime>
#include "unistd.h"

using namespace std;


class Logger
{
public:
    Logger();
    ~Logger();
    string getTime(int);
    void log(string data);

private:
    ofstream logfile;
};

#endif // LOG

