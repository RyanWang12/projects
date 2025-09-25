#ifndef __COMMENTNOTIFICATION_H
#define __COMMENTNOTIFICATION_H
#include "Notification.h"
#include <fstream>
#include <iostream>

class CommentNotification: public Notification{
    public:
    CommentNotification(): Notification(){}
    // return comment notification
        virtual void notifMessage(const std::string& author, std::ofstream& outFile) override{
            outFile<< author +"commented on your post."<<"\n";
        }
    
    
};
#endif