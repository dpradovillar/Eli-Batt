#include <iostream>
#include "seriallib.h"
using namespace std;

int main(int argc, char *argv[])
{


    SerialLib serialLib;
    ThreadedSerial threadedSerial(&serialLib);


    if (serialLib.begin("COM6", 9600)) {
        cout << "connection open!" << endl;
    } else {
        cout << "error: connection not open" << endl;
    }

    while(serialLib.available()) {
        cout << (char) serialLib.read();
    }

    cout << "finishing application" << endl;

    return 0;
}
