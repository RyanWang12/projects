#ifndef __USER_H
#define __USER_H
#include <string>

class User{
    public:
        User(){}
        // overloaded constructor to create user object
        User(const bool hasPauseAllNotif,
            const bool hasLikesNotif,
            const bool hasTagsNotif,
            const bool hasCommentsNotif,
            const bool hasFollowersNotif,
            const bool hasMsgRequestNotif):
                hasPauseAllNotif(hasPauseAllNotif),
                hasLikesNotif(hasLikesNotif),
                hasTagsNotif(hasTagsNotif),
                hasCommentsNotif(hasCommentsNotif),
                hasFollowersNotif(hasFollowersNotif),
                hasMsgRequestNotif(hasMsgRequestNotif){}


        // getters for each preference
        bool getPauseAllNotif()const {return hasPauseAllNotif;}
        bool getLikesNotif()const {return hasLikesNotif;}
        bool getTagsNotif()const {return hasTagsNotif;}
        bool getCommentsNotif()const {return hasCommentsNotif;}
        bool getFollowersNotif()const {return hasFollowersNotif;}
        bool getMsgNotif()const {return hasMsgRequestNotif;}

    private:
        // variables to store notification preference
        bool hasPauseAllNotif;
        bool hasLikesNotif;
        bool hasTagsNotif;
        bool hasCommentsNotif;
        bool hasFollowersNotif;
        bool hasMsgRequestNotif;
};


#endif