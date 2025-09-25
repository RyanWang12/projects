#include<string>
#include<vector>
#ifndef __USER_H
#define __USER_H

class User{
    public:
        // overloaded constructor
        User(std::string name, int age, std::string gender,
            std::string phoneNum, std::string profession,
            std::string school, double latit, double longit,
            std::string isPremium, int minAgePref,
            int maxAgePref, double maxDistancePref,
            std::string genderPref, std::vector<std::string> likedUsers);
    

        // variables to store info from file
        std::string name;
        int age;
        std::string gender;
        std::string phoneNum;
        std::string profession;
        std::string school;
        double latitude;
        double longititude;
        std::string isPremium;
        int minAgePref;
        int maxAgePref;
        double maxDistancePref;
        std::string genderPref;
        std::vector<std::string> likedUsers; // stores each number this user liked
};




#endif