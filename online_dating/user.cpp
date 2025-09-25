#include "user.h"
#include <string>
#include <vector>

// overloaded constructor to take info from file
User:: User(std::string name, int age, std::string gender,
         std::string phoneNum, std::string profession,
         std::string school, double latit, double longit,
         std::string isPremium, int minAgePref,
         int maxAgePref, double maxDistancePref,
         std::string genderPref, std::vector<std::string> likedUsers):
             name(name), age(age),
             gender(gender),phoneNum(phoneNum),
             profession(profession), school(school),
             latitude(latit), longititude(longit),
             isPremium(isPremium), minAgePref(minAgePref),
             maxAgePref(maxAgePref), maxDistancePref(maxDistancePref),
             genderPref(genderPref), likedUsers(likedUsers){};