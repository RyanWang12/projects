#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "user.h"
#include "node.h"

/*
TODO: finish writing push_back function
      check readFile() works
      check getLikeUsers() works

*/

// calculate distance between 2 locations in miles
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double radiusOfEarth = 6371.0; // Earth's radius in kilometers

    // convert latitude and longitude from degrees to radians
    lat1 *= M_PI / 180.0;
    lon1 *= M_PI / 180.0;
    lat2 *= M_PI / 180.0;
    lon2 *= M_PI / 180.0;

    // Haversine formula
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    double a = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(lat1) * cos(lat2) * sin(dLon / 2.0) * sin(dLon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    // distance in kilometers
    double distanceKM = radiusOfEarth * c;
    // convert it to distance in miles
    double distanceMiles = distanceKM * 0.621371;

    return distanceMiles;
}


//read the string that contains all users this user liked, and store their phone number into a vector
std::vector<std::string> getLikedUsers(std::string likedUserLine){
    std::vector<std::string> likedUsers; //  store all users that this user liked
    std::string longString = likedUserLine;

     // create a stringstream to tokenize the long string
    std::istringstream iss(longString);
    std::string phoneNumber;

    // read through the liked users and store them into a vector
    while (std::getline(iss, phoneNumber, '_')) {
        likedUsers.push_back(phoneNumber);
    } 
    return likedUsers;
}

// this will add a new node to end of this list
void push_back(Node*& head, const User& userInfo){
    // check if head is empty (pointing to null)
    if(head == nullptr){
        head = new Node(userInfo);
    }
    
    else{
        Node* currNode = head; // pointer to traverse through list

        // traverse to end of list
        while(currNode->next != nullptr){
            currNode = currNode->next;
        }
        // add a new node to the end of this list
        currNode->next = new Node(nullptr, currNode, userInfo);
        
    }
    
}

// read and store info from file into list
void readFile(std::ifstream& inFile, Node*& head){
        //variable initialization
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

        // line that stores other user's phone num that this user liked
        std::string likedUsersPhoneNums;

        // vector to store all the user numbers that this user liked 
        std::vector<std::string> likedUsersVec;

        // read the file
        while(inFile >> name >> age >>
                       gender >>phoneNum >>
                       profession >> school >>
                       latitude >> longititude >>
                       isPremium >> minAgePref >> 
                       maxAgePref >> maxDistancePref >>
                       genderPref >> likedUsersPhoneNums ){
            likedUsersVec = getLikedUsers(likedUsersPhoneNums);

            // store each line's info into user
            User user(name, age,
                      gender, phoneNum,
                      profession, school,
                      latitude, longititude,
                      isPremium, minAgePref,
                      maxAgePref, maxDistancePref,
                      genderPref, likedUsersVec);

            // add each user info into list
            push_back(head, user);
           
        }
}

// get the user specified from command line
Node* getUser(Node*& head, const std::string& phoneNum){
    // check if list is empty
    if(head == nullptr){
        return head;
    }

    Node* current = head;
    // traverse list & check which profile matches this phone number
    while(current != nullptr){
        if(current->userInfo.phoneNum == phoneNum){
            return current;
        }
        current = current->next;
    }
    return nullptr; // return null if user not found
}

// remove & replace specified character from given string 
void replaceCharInString(std::string& str, char charToRemove, char replacementChar) {
    std::replace(str.begin(), str.end(), charToRemove, replacementChar);
}

// output profile that matches a user's preference
void outputProfile(std::ofstream& outFile, std::vector<User> userProfiles){

    // loop through each user profile
    for(unsigned int i = 0; i<userProfiles.size(); i++){
        User thisUser = userProfiles[i];

        //remove '_' from school & profession
        replaceCharInString(thisUser.profession, '_', ' ');
        replaceCharInString(thisUser.school, '_', ' ');

        // print out name, age, profession & school (if given)
        outFile<<thisUser.name<<" "<<thisUser.age<<std::endl;
        if(thisUser.profession != "Undisclosed"){
            outFile<<thisUser.profession<<std::endl;
        }
        
        if(thisUser.school != "Undisclosed"){
             outFile<<thisUser.school<<std::endl;
        }

        // end line unless it's the last element
        if(i != userProfiles.size()-1){
            outFile<<std::endl;
        }

    }
}


// check if the other user has the preferences of this user
bool isMatchesPreference(Node*& thisUser,Node*& otherUser){
    // store the preference of this user
    int minAgePref = thisUser->userInfo.minAgePref;
    int maxAgePref = thisUser->userInfo.maxAgePref;
    std::string genderPref = thisUser->userInfo.genderPref;
    double distanceApart = calculateDistance(thisUser->userInfo.latitude,
                                          thisUser->userInfo.longititude, 
                                          otherUser->userInfo.latitude,
                                          otherUser->userInfo.longititude);

    // if the profiles are of the same user, don't compare
    if(thisUser == otherUser){
        return false;
    }

    // if gender preference is both, check only if other user's age & distance between these users
    else if(genderPref == "Both"){
        return ((otherUser->userInfo.age >= minAgePref && otherUser->userInfo.age<=maxAgePref)
                 && distanceApart <= thisUser->userInfo.maxDistancePref);
    }

    // if gender preference matters, check the age, gender, & distance between these users
    return ((otherUser->userInfo.age >= minAgePref && otherUser->userInfo.age<=maxAgePref)
            && otherUser->userInfo.gender == genderPref
            && distanceApart <= thisUser->userInfo.maxDistancePref); 
}


// comparator to sort by smallest phone num
bool bySmallestPhoneNum(const User& thisUser, const User& otherUser ){
    //variable initialization
    std::string thisPhoneNum = thisUser.phoneNum;
    std::string otherPhoneNum = otherUser.phoneNum;
    int thisNum; // store each integer in this phone num
    int otherNum; // // store each integer in other phone num

    // look through each value in phone num
    for(unsigned int i = 0; i<thisPhoneNum.size();++i){
        if(thisPhoneNum[i] != '-'){

            //compare the integer value of each character
            thisNum = thisPhoneNum[i] - '0';
            otherNum = otherPhoneNum[i] - '0';

            // if this integer is greater than other, return true
            if(thisNum<otherNum){
                return true;
            }
            // if this integer is greater than the other, return false to sort
            else if(thisNum > otherNum){
                return false;
            }
        }
    }
    return false;   
}

// show the profiles that matches given user's preferences in age, gender, and distance
void showProfile(Node*& head, std::ofstream& outFile, std::string& phoneNum){
    // variable initialization
    Node* thisUser = getUser(head, phoneNum); // find this user from phone num
    Node* current = head;
    bool atLeastOneMatch = false;
    std::vector<User> likedProfiles;

    if(head == nullptr){ // check if list is empty
        return;
    }
    
    if(thisUser == nullptr){ // check if user is found
        return;
    }

    // traverse through list
    while(current != nullptr){
        // check if current user matches with this user's preference in age, gender, and within distance
        if(isMatchesPreference(thisUser, current)){
                atLeastOneMatch = true; 
                likedProfiles.push_back(current->userInfo);
                         
            } 
        current = current->next;
    }

    // check if no one matches this user's preference
    if(atLeastOneMatch == false){
        outFile<<"There are no users matching with your preference at this moment."<<std::endl;
    }
    else{
        outputProfile(outFile,likedProfiles);
    }

}

// check if both given users like each other
bool bothSwipedRight(Node*& thisUser, Node*& otherUser){
    // variable initialization
    std::vector<std::string> likedByThisUser = thisUser->userInfo.likedUsers; // store users this user likes
    std::vector<std::string> likedByOtherUser = otherUser->userInfo.likedUsers; // store users other user likes
    std::string thisUserPhoneNum = thisUser->userInfo.phoneNum;
    std::string otherUserPhoneNum = otherUser->userInfo.phoneNum;
    bool thisUserLikes = false;
    bool otherUserLikes = false;

    // if one or both users have no liked users, return false
    if(likedByThisUser[0] == "null" || likedByOtherUser[0] == "null"){
        return false;
    }
    // don't check if both users refer to the same person
    else if(thisUser == otherUser){
        return false;
    }

    // check if this user is liked by the other user
    for(unsigned int i =0; i< likedByOtherUser.size(); ++i){
        if(thisUserPhoneNum == likedByOtherUser[i]){
            otherUserLikes = true;
        }
    }

    // check if the other user is liked by this user
    for(unsigned int i =0; i<likedByThisUser.size(); ++i){
        if(otherUserPhoneNum == likedByThisUser[i]){
            thisUserLikes = true;
        }
    }

    // if both users like each other, then return true
    if(otherUserLikes == true && thisUserLikes == true){
        return true;
    }
    return false;

}

// if given phone number is found within match list, remove it
void removeMatch(const std::string& phoneNum, std::vector<User>& userMatches){
    int indexToRemove = -1; // store index to remove the phone num

    // check if given phone number exist within the user's match list
    for(unsigned int i = 0; i<userMatches.size(); ++i){
        if(userMatches[i].phoneNum == phoneNum){
            indexToRemove = i;
        }
    }

    // if phone number is found, remove that user from match list
    if(indexToRemove > -1){
        userMatches.erase(userMatches.begin()+indexToRemove);
    }
}

// check if other users likes this user
bool userSwipedRight(const std::string& phoneNum, Node* otherUser){
    std::vector<std::string> likedByOtherUser = otherUser->userInfo.likedUsers;
    
    // check if other user has liked this user
    for(unsigned int i =0; i<likedByOtherUser.size(); ++i){
        if(likedByOtherUser[i] == phoneNum){
            return true;
        }
    }
    return false;
}

// output the users who like this user, and this user also like the other user
void showMatches(Node*& head, std::ofstream& outFile,const std::string& phoneNum){
    Node* thisUser = getUser(head, phoneNum); // find this user from phone num
    Node* current = head;
    bool atLeastOneMatch = false;
    std::vector<User> userMatches;

    if(head == nullptr){ // check if list is empty
        return;
    }
        
    if(thisUser == nullptr){ // check if user is found
        return;
    }

    // traverse through list
    while(current != nullptr){
        // if both users swiped right, output the match
        if(bothSwipedRight(thisUser, current)){ // check if both users swiped right
            userMatches.push_back(current->userInfo);
            atLeastOneMatch = true;
        }
        current = current->next;
    }

    // if no matches found, print this out
    if(atLeastOneMatch == false){
        outFile<<"You do not have any matches at this moment."<<std::endl;
    }
    // if matches found, sort by ascending phone num order
    else{
        std::sort(userMatches.begin(),userMatches.end(), bySmallestPhoneNum);
        outputProfile(outFile,userMatches);
    }

}

// show the users who've swiped right on given user
void showLikes(Node*& head, std::ofstream& outFile, const std::string& phoneNum){
    //variable initialization
    Node* thisUser = getUser(head, phoneNum); // find this user from phone num
    Node* current = head;
    bool atLeastOneMatch = false; 
    std::vector<User> swipedRightUsers; // store users who this user likes, and vice versa

    if(head == nullptr){ // check if list is empty
        return;
    }

    if(thisUser == nullptr){ // check if user is found
        return;
    }

    if(thisUser->userInfo.isPremium == "false"){ // check if user is premium
        outFile<<"Only premium users can view who liked you."<<std::endl;
        return;
    }

    // traverse through list
    while(current != nullptr){
        // if this user swiped right, output the user
        if(userSwipedRight(phoneNum, current) == true){
            swipedRightUsers.push_back(current->userInfo);
            atLeastOneMatch = true;
        }
        current = current->next;
    }

    // if no users swiped right, print this out
    if(atLeastOneMatch == false){
        outFile<<"You have not received any likes so far."<<std::endl;
    }
    else{
       outputProfile(outFile,swipedRightUsers); 
    }

}


// delete the list & free up memory in heap
void deleteList(Node* & head){
    Node* current = head;
    if(head == nullptr){ // check if head is not empty
        return;
    }

    // delete each node 
    while(head != nullptr){
        head = head->next;
        delete current;
        current = head;
    }
}

// output the updated match list after these 2 users
void outputUnmatch(std::ofstream& outFile, 
                   const std::string thisUserName,
                   const std::string otherUserName,
                   const std::vector<User>& thisUserMatches,
                   const std::vector<User>& otherUserMatches ){
    
   
    // output no matches if both users only have this one match
    if(thisUserMatches.size() == 0 && otherUserMatches.size() == 0){
        outFile<<thisUserName<<"'s "<<"match list:"<<std::endl;

        outFile<<std::endl;
        outFile<<"You do not have any matches at this moment."<<std::endl;
        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;


        outFile<<otherUserName<<"'s "<<"match list:"<<std::endl;
        outFile<<std::endl;
        outFile<<"You do not have any matches at this moment."<<std::endl;
    }
    
    // output no matches for thisUser if they only have this one match
    else if(thisUserMatches.size() == 0 && otherUserMatches.size() > 0){
        outFile<<thisUserName<<"'s "<<"match list:"<<std::endl;

        outFile<<std::endl;
        outFile<<"You do not have any matches at this moment."<<std::endl;
        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<otherUserName<<"'s "<<"match list:"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile,otherUserMatches);
    }
    
    // output no matches for otherUser if they only have this one match
    else if(otherUserMatches.size() == 0 && thisUserMatches.size() > 0){
        outFile<<thisUserName<<"'s "<<"match list:"<<std::endl;

        outFile<<std::endl;
        outputProfile(outFile,thisUserMatches);
        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<otherUserName<<"'s "<<"match list:"<<std::endl;
        outFile<<std::endl;
        outFile<<"You do not have any matches at this moment."<<std::endl;
    }
    
    // output both user's match list if both of their list is more than 0
    else{
        outFile<<thisUserName<<"'s "<<"match list:"<<std::endl;

        outFile<<std::endl;
        outputProfile(outFile,thisUserMatches);
        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<otherUserName<<"'s "<<"match list:"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile,otherUserMatches);
    }
    
}


// find the user's match list and remove specified matched user
void unmatch(Node*& head, std::ofstream& outFile,
             const std::string& thisPhoneNum,
             const std::string& otherPhoneNum){

    //variable initialization
    Node* thisUser = getUser(head, thisPhoneNum); 
    Node* otherUser = getUser(head, otherPhoneNum); 
    Node* current = head;

    // store the matches for first  & second user
    std::vector<User> thisUserMatches;
    std::vector<User> otherUserMatches;
    

    if(head == nullptr){ // check if list is empty
        return;
    }
        
    if(thisUser == nullptr || otherUser == nullptr){ // check if both users are found
        return;
    }

    // traverse through list
    while(current != nullptr){
        // get the match lists for both users
        if(bothSwipedRight(thisUser, current)){ // check if both users swiped right
            thisUserMatches.push_back(current->userInfo);
        
        }

        if(bothSwipedRight(otherUser, current)){
            otherUserMatches.push_back(current->userInfo);
        }
        
        current = current->next;
    }

    // remove specified users from each other's list
    removeMatch(otherUser->userInfo.phoneNum, thisUserMatches);
    removeMatch(thisUser->userInfo.phoneNum, otherUserMatches);

    std::sort(thisUserMatches.begin(), thisUserMatches.end(), bySmallestPhoneNum);
    std::sort(otherUserMatches.begin(), otherUserMatches.end(), bySmallestPhoneNum);

    //output the updated match list for both users
    outputUnmatch(outFile, thisUser->userInfo.name,
                  otherUser->userInfo.name,thisUserMatches,
                  otherUserMatches);
}


// output the updated profile list for each use
void outputBlock(std::ofstream& outFile, 
                const std::string thisUserName,
                const std::string otherUserName,
                const std::vector<User>& thisUserProfilesList,
                const std::vector<User>& otherUseProfileList){

    // if both users only have this one match, print out no matching users with preference
    if(thisUserProfilesList.size() == 0 && otherUseProfileList.size() == 0){
        outFile<<"profiles shown to "<<thisUserName<<":"<<std::endl;
        outFile<<std::endl;
        outFile<<"There are no users matching with your preference at this moment."<<std::endl;

        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<"profiles shown to "<<otherUserName<<":"<<std::endl;
        outFile<<std::endl;
        outFile<<"There are no users matching with your preference at this moment."<<std::endl;
    }

    // if thisUser has no profile list, print out no match & print out other user's profile list
    else if(thisUserProfilesList.size() == 0 && otherUseProfileList.size() > 0){
        outFile<<"profiles shown to "<<thisUserName<<":"<<std::endl;
        outFile<<std::endl;
        outFile<<"There are no users matching with your preference at this moment."<<std::endl;

        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<"profiles shown to "<<otherUserName<<":"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile,otherUseProfileList);

    }
    // if otherUser has no profile list, pritn out no match & print out this user's profile list
    else if(otherUseProfileList.size() == 0 && thisUserProfilesList.size() > 0){
        outFile<<"profiles shown to "<<thisUserName<<":"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile, thisUserProfilesList);

        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<"profiles shown to "<<otherUserName<<":"<<std::endl;
        outFile<<std::endl;
        outFile<<"There are no users matching with your preference at this moment."<<std::endl;
    }
    // print both user's profile list 
    else{
        outFile<<"profiles shown to "<<thisUserName<<":"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile, thisUserProfilesList);

        outFile<<std::endl;
        outFile<<"======"<<std::endl;
        outFile<<std::endl;

        outFile<<"profiles shown to "<<otherUserName<<":"<<std::endl;
        outFile<<std::endl;
        outputProfile(outFile,otherUseProfileList);
    }   
}

// remove the specified users from each other's list
void block(Node*& head, std::ofstream& outFile,
            const std::string& thisPhoneNum,
            const std::string& otherPhoneNum){

    //variable initialization
    Node* thisUser = getUser(head, thisPhoneNum); // find this user from phone num
    Node* otherUser = getUser(head, otherPhoneNum);
    Node* current = head;
    std::vector<User> thisUserProfileList; // store profiles that match this user's preference
    std::vector<User> otherUserProfileList; // store profiles that match other user's preference


    if(head == nullptr){ // check if list is empty
        return;
    }

    if(thisUser == nullptr){ // check if user is found
        return;
    }

    // traver through users list
    while(current != nullptr){
        // find the profiles that match this user's preference
        if(isMatchesPreference(thisUser, current)){
            thisUserProfileList.push_back(current->userInfo);
        }

        // // find the profiles that match other user's preference
        if(isMatchesPreference(otherUser, current)){
            otherUserProfileList.push_back(current->userInfo);
        }
        current = current->next;
    }

    // remove the profile from each other's profile lists
    removeMatch(otherUser->userInfo.phoneNum, thisUserProfileList);
    removeMatch(thisUser->userInfo.phoneNum, otherUserProfileList);

    // print out the updated profile list
    outputBlock(outFile, thisUser->userInfo.name,
                otherUser->userInfo.name,thisUserProfileList,
                otherUserProfileList);
}
int main(int argc, char*argv[]){
    
    // check if number of arguments is less than 5
    if(argc < 5){
        std::cerr<<"usage: ./nydate.exe users.txt output.txt phoneNumber command"<<std::endl;
    }   

    // variable initialization
    Node* users = nullptr; // store all user info into this list
    std::ifstream inFile(argv[1]); 
    std::ofstream outFile(argv[2]);
    std::string command = argv[4]; 
    std::string phoneNum = argv[3]; // store give phone num
    std::string otherPhoneNum;


    //read the file & put them into a user list
    readFile(inFile, users);
  
    // check what command it is
    if(command == "profile"){
        showProfile(users,outFile,phoneNum);
    }
    else if(command == "match"){
        showMatches(users, outFile,phoneNum);
    }
    else if(command == "like"){
        showLikes(users, outFile, phoneNum);
    }
    else if(command == "unmatch"){
        otherPhoneNum = argv[5];
        unmatch(users,outFile,phoneNum,otherPhoneNum);
    }
    else if(command == "block"){
        otherPhoneNum = argv[5];
        block(users,outFile,phoneNum,otherPhoneNum);
    }
    else{
        std::cerr<<"Invalid command: "<<command<<std::endl;
    }
    
    deleteList(users);
    return 0;
}