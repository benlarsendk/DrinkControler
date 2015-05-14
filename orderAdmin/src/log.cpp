#include "log.h"
#include "unistd.h"


Logger::Logger()
{
   const char * pluspath = ("/home/deblab/Desktop/" + getTime(0)).c_str();
   logfile.open(pluspath, fstream::app);
}


Logger::~Logger()
{
    logfile.close();
}
void Logger::log(string data)
{
    logfile << getTime(1) << ":\t" << data << "\n";
}
string Logger::getTime(int timedate)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    if (timedate == 1){
        strftime(buffer,80,"%d/%m-%Y@%H:%M:%S",timeinfo);
        string str(buffer);
        return str;
    }
    else{
        strftime(buffer,80,"%d%m%Y",timeinfo);
        string str(buffer);
        return str;
    }

}
