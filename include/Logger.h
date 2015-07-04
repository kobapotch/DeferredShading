#ifndef _KOBA_LOGGER_INCLUDED
#define _KOBA_LOGGER_INCLUDED

#include <iostream>

using namespace std;

class Logger{
public:
    static void Log(string str){
        cout << str << endl;
    }
};

#endif
