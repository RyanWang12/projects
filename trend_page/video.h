#include <string>
#include <map>
#include <vector>
#include <fstream>
#ifndef __VIDEO_H
#define __VIDEO_H

class Video{
    public:
        // constructor
        Video(const long &views, const std::string& cURL,
              const std::string& wURL, std::string id=""): 
             numViews(views), coverURL(cURL), webVideoURL(wURL), id(id){};

        

        // getters for each variable
        std::string getCoverURL()const {return coverURL;}
        std::string getWebVideoURL()const {return webVideoURL;}
        long getViews()const{return numViews;}
        std::string getID()const {return id;}

    private:
        long numViews;
        std::string coverURL;
        std::string webVideoURL;
        std::string id;


};




#endif