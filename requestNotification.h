#ifndef __REQUESTNOTIFICATION_H
#define __REQUESTNOTIFICATION_H
#include "Notification.h"
#include <iostream>
#include <fstream>

class RequestNotification: public Notification{
    public:
        RequestNotification(): Notification(){}

        // ouput to file given the author name
        void notifMessage(const std::string& author, std::ofstream& outFile) override{
            outFile<< author +"wants to send you a message."<<"\n";
        }
        
    private:

};

#endif