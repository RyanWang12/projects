#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include "user.h"
#include "Notification.h"
#include "followNotification.h"
#include "tagNotification.h"
#include "likeNotification.h"
#include "requestNotification.h"
#include "commentNotification.h"

// nynotifications.exe posts.json users.json events.txt output.txt username
std::unordered_map<std::string, User> users; // stores user data
std::unordered_map<std::string, std::string> posts; // stores post data
std::stack<std::pair<std::string, char> > notifications; // stores the notifications

// remove the quotes at the beginning and end of a string
std::string removeQuotes(const std::string& line){
    // check for ending quotes
    if(line.front() == '"' && line.back() == '"'){
        return line.substr(1, line.size()-2); // remove quotes
    }
    // did not find quotes, return line
    return line;
}

// return the body between start and end word of a json line
std::string getInfo(const std::string& line, 
                    const std::string& startWord,
                    const std::string& endWord){
    size_t startPos = line.find(startWord); // find starting position of keyword
    size_t endPos;
    std::string infoLine;

    // if keyword not found, early return
    if(startPos == std::string::npos){
        return " ";
    }
    else{   
        startPos += startWord.size(); // skip the keword
        if(line[startPos] == ' '){
            startPos++;
        }
        // find ending portion of the body
        infoLine = line.substr(startPos);
        endPos = infoLine.find(endWord);
        
    }
    return removeQuotes(line.substr(startPos, endPos));
    
}
// convert the string version of "true" or "false" to boolean
bool strToBool(std::string boolean){
    if(boolean == "true"){
        return true;
    }
    return false;  
}

// read user json file
void readUserData(std::ifstream& inFile ){
    std::string line;
    std::string notifMeta; // store the notification settings
    while(std::getline(inFile, line)){
        // get the notification setting of each user
        notifMeta = getInfo(line,"\"notifications\":", "]");
        // create user object
        User u(strToBool(getInfo(notifMeta, "\"pauseAll\":", ",")),
               strToBool(getInfo(notifMeta, "\"likes\":", ",")),
               strToBool(getInfo(notifMeta, "\"tags\":", ",")),
               strToBool(getInfo(notifMeta, "\"comments\":", ",")),
               strToBool(getInfo(notifMeta, "\"newFollowers\":", ",")),
               strToBool(getInfo(notifMeta, "\"messageRequests\":", "}")));

        // store each user id with its user preference
        users.insert(std::make_pair(getInfo(line, "\"username\":", ","), u)); 

    }
}

// read the post json file
void readPostData(std::ifstream& inFile){
    std::string line;
    while(std::getline(inFile, line)){

        // insert post id with its associated user name
        posts[getInfo(line, "\"id\":","," )] = getInfo(line,"\"ownerUsername\":","," ); 
        if(getInfo(line,"\"ownerUsername\":","," ) == "justinbieber"){

        }  
    }
}

// check if a user wants to recieve a given notification
bool hasNotifOn(const char& operation, const std::string& name){
    User u = users[name]; 
    if(operation == 'l'){ // check like
        return u.getLikesNotif();
    }
    else if(operation == 't'){ // check tag
        return u.getTagsNotif();
    }
    else if(operation == 'f'){ // check followers
        return u.getFollowersNotif();
    }
    else if(operation == 'c'){ // check comments
        return u.getCommentsNotif();
    }
    return u.getMsgNotif(); // check message
}



// read eventFile to find notifications associated with given user name
void getNotificationFor(std::ifstream& eventFile,const std::string& givenUser){

    // variable initialization
    std::string thisUser;
    std::string otherUser;
    std::string operation; 
    char operLetter;
    std::unordered_map<std::string, std::string>::iterator p_ptr;

    // if user has pausesAll notifications, return early
    if(users[givenUser].getPauseAllNotif() == true){
        return;
    }
    // parse through event file
    while(eventFile >> thisUser >> operation >> otherUser){
        operLetter = operation[0]; // get first letter of operation
        // check if operation is likes or comments_on 
        if(operation == "likes" || operation == "comments_on" ){
            // check if this post can be found
            p_ptr = posts.find(otherUser);

            if(p_ptr == posts.end()){
                continue;
            }
            // check if this username matches given username
            if(p_ptr->second != givenUser){
                continue;
            }
            // check if the user wants to recieve this notification
            if(hasNotifOn(operLetter, p_ptr->second) == false){
                continue;
            }
            // givenUser should recieve this notification, push into stack
            notifications.push(std::make_pair(thisUser, operLetter));
        }    
        else{   
            //check if other username match given username
            if(otherUser != givenUser){
                continue;
            }

            // check if the user wants to recieve this notification
            if(hasNotifOn(operLetter, otherUser) == false){
                continue;
            }

            // push thisUser & operation character into stack
            notifications.push(std::make_pair(thisUser, operLetter));
        }          
   }
}

// output the given notifications to output file
void outputNotifications(const std::vector<std::string>& sameNotifs, 
                         const char& notifType,
                         std::ofstream& outFile,
                         int& count){

    // variable initiaization
    std::string firstNames = "";
    FollowNotification follow;
    LikeNotification like;
    TagNotification tag;
    RequestNotification msgRequest;
    CommentNotification comment;
    int outputCount = 101 - count;

    // check if there's more notifications to output
    if(outputCount <=0){
        return;
    }

    // if 3 or less notifications, just output each as individual notifications
    if(sameNotifs.size() <= 3){

        // if needed # of outputs is greater than # of given notifications,
        // output the # of given notifications
        if(outputCount >= (int) sameNotifs.size()){
            outputCount = sameNotifs.size();
            count+= outputCount;
        }
        // if needed # of outputs is less than given # of output,
        // output # of remaining outputs
        else{
            count+= outputCount;
        }

        // output apprioriate # of outputs
        for( int i = 0; i<outputCount; ++i){
            // find the apprioriate notifications to output
            if(notifType == 'f'){// follow notification
                follow.notifMessage(sameNotifs[i],outFile);
            }
            else if(notifType == 'l'){// like notification
                like.notifMessage(sameNotifs[i],outFile);
            }
            else if(notifType == 'c'){// comment notification
                comment.notifMessage(sameNotifs[i],outFile);
            }
            else if(notifType == 'm'){// msg request notification
                msgRequest.notifMessage(sameNotifs[i],outFile);
            }
            else if(notifType == 't'){ // tag notification
                tag.notifMessage(sameNotifs[i],outFile);
            }
        }

    }
    else{
        count++;
        std::string notifSize = std::to_string((int)sameNotifs.size()-2); 

        // aggregate msg into the first 2 names along with rest of the other people
        std::string tmp = sameNotifs[0];
        tmp = tmp.substr(0,tmp.size()-1);

        firstNames +=tmp + ", "+sameNotifs[1] + "and " + notifSize + " others "; 

        // find the appropriate notification to send out
        if(notifType == 'f'){ // follow notification
            follow.notifMessage(firstNames,outFile);
        }
        else if(notifType == 'l'){ // like notification
            like.notifMessage(firstNames,outFile);
        }
        else if(notifType == 'c'){ // comment notification
            comment.notifMessage(firstNames,outFile);
        }
        else if(notifType == 'm'){ // msg request notification
            msgRequest.notifMessage(firstNames,outFile);
        }
        else if(notifType == 't'){ // tag notification
            tag.notifMessage(firstNames,outFile);
        }
        
    }
}

// combine consecutive notifications of the same type into 1 line
void aggregateNotifications(std::stack<std::pair<std::string, char> >& n, std::ofstream& outFile){
    std::vector<std::string> sameNotifs;
    char prevNotif;
    int notifCount = 1;

    // if there's no notifications, early return
    if(n.size() == 0){
        return;
    }
    // if size is only one, just output this notification
    else if(n.size() == 1){ 
        prevNotif = n.top().second;
        sameNotifs.push_back(n.top().first+" ");
        outputNotifications(sameNotifs,n.top().second, outFile, notifCount);
        n.pop();
        return;
    }
    else{ // if stack is not empty, get the first item
        prevNotif = n.top().second;
        sameNotifs.push_back(n.top().first+" ");
        n.pop();

    }
    
    // output notifications until it hits 100
    while((notifCount != 100) && (n.empty() == false)){
        // if consecutive operations occur, push it into vector
        if(prevNotif == n.top().second){  
            sameNotifs.push_back(n.top().first + " ");
        }
        // not consecutive operations,output the notifications
        else{
            outputNotifications(sameNotifs,prevNotif,outFile,notifCount);
            prevNotif = n.top().second; // set notification current type
            sameNotifs.clear();
            sameNotifs.push_back(n.top().first +" "); // add new notification

        }
        n.pop();
    }
     // output rest of notifications, if any
    outputNotifications(sameNotifs,prevNotif,outFile,notifCount); 

}

int main(int argc, char* argv[]){

    if(argc != 6){
        std::cerr<<"usage: nynotifications.exe posts.jsonl users.jsonl events.txt output.txt username"<<"\n";
        return 1;
    }

    // variable initialization
    std::ifstream postFile(argv[1]);
    std::ifstream userFile(argv[2]);
    std::ifstream eventFile(argv[3]);
    std::ofstream outFile(argv[4]);

    readUserData(userFile); // read user data
    readPostData(postFile); // read post data
    getNotificationFor(eventFile, argv[5]); // read event file


    aggregateNotifications(notifications,outFile); // output notifications

    return 0;
}