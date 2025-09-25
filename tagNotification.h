#ifndef __TAGNOTIFICATION_H
#define __TAGNOTIFICATION_H
#include "Notification.h"
#include <iostream>
#include <fstream>

class TagNotification: public Notification{
    public:
        TagNotification(): Notification(){}

        // ouput to file given the author name
        void notifMessage(const std::string& author, std::ofstream& outFile) override{
            outFile<< author +"tagged you in a post."<<"\n";
        }
        
    private:

};

#endif