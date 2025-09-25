#ifndef __NOTIFICATION_H
#define __NOTIFICATION_H
#include <string>
#include <iostream>
#include <fstream>
class Notification{
    public:
        // constructors
        Notification(){}

        virtual void notifMessage(const std::string& author, std::ofstream& outFile){outFile<< author<<"\n";} // print out to file
    
    private:
};


#endif