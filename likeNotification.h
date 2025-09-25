#ifndef __LIKENOTIFICATION_H
#define __LIKENOTIFICATION_H
#include "Notification.h"
#include <fstream>
#include <iostream>

class LikeNotification: public Notification{
    public:
    LikeNotification(): Notification(){}
    // return comment notification
        void notifMessage(const std::string& author, std::ofstream& outFile) override{
            outFile<< author +"liked your post."<<"\n";
        }
    
    
};
#endif