#ifndef LOG
#define LOG

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <ctime>
#include <boost/thread.hpp>

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
    boost::mutex mtx_;

};


#endif // LOG

