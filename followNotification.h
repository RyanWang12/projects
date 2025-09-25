#ifndef __FOLLOWNOTIFICATION_H
#define __FOLLOWNOTIFICATION_H
#include "Notification.h"
#include <fstream>
#include <iostream>
class FollowNotification: public Notification{
    public:
        FollowNotification(): Notification(){}

        // return follow notification
        void notifMessage(const std::string& author, std::ofstream& outFile) override{
           outFile<<author +"started following you."<<"\n";
        }
    private:
 
};


#endif