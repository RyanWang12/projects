#include <string>
#include <iostream>
#include <queue>
#include <regex>
#include <fstream>
#include "video.h"
#include "trendStat.h"
#include "musicInfo.h"
#include <unordered_map>

// store the stats of each hashtag (name, # of videos, etc)
typedef std::pair<std::string, TrendStat>  hashStats;
typedef std::pair<musicInfo, TrendStat> soundStats;

// comparator class used for sorting hastags in priority queue
class BiggerHashStat{
    public:
        bool operator()(const hashStats& a, const hashStats& b){

            // if not the same usage number, compare by usage
            long a_usage = a.second.getUsageCount();
            long b_usage = b.second.getUsageCount();
            if(a_usage != b_usage){
                return a_usage < b_usage;
            }

            // if not the same total views number, compare by total views
            long a_total = a.second.getTotalViews();
            long b_total = b.second.getTotalViews();
            if(a_total != b_total){
                return a_total < b_total;
            }

            // compare by name of hashtag
            return a.first > b.first;
        }
};

// comparator class used for sorting sound in priority queue
class BiggerSoundStat{
    public:
        bool operator()(const soundStats& a, const soundStats& b){

            // if not the same total views number, compare by total views
            long a_total = a.second.getTotalViews();
            long b_total = b.second.getTotalViews();
            if(a_total != b_total){
                return a_total < b_total;
            }

            // compare by music id
            return a.first.getId() > b.first.getId();
        }
};

// string stores the # name, the total views of all the videos, and the videos in this category
//vector stores each video with this string
typedef std::unordered_map<std::string, TrendStat>  hashInfo;
typedef std::unordered_map<musicInfo, TrendStat> soundInfo;

hashInfo hashtagInfo;
soundInfo soundsInfo;

typedef std::priority_queue<hashStats, std::vector<hashStats>, BiggerHashStat> hashTrend;
typedef std::priority_queue<soundStats, std::vector<soundStats>, BiggerSoundStat >soundTrend;

// get a section of string of line, given a start and end word
std::string getInfo(const std::string& line, const std::string startWord){
    size_t startPos = line.find(startWord);
    size_t endPos;

    // if keyword not found, exit
    if(startPos == std::string::npos){
        return " ";
    }
    // keyword exists, so skip the word and get the body of it
    else{
        startPos+= startWord.size()+2;
    }

    // find the end of the key-value pair 
    std::string tmp = line.substr(startPos);
    endPos = tmp.find("\",");

    // return the given section
    return tmp.substr(0,endPos);

}

// get the cover url of each video in file
std::string getCoverURL(const std::string& line){
    return getInfo(line, "\"coverUrl\":");
}

// get the web url of each video in file
std::string getWebVideoURL(const std::string& line){
    return getInfo(line, "\"webVideoUrl\":");
}

// get video if of each line in file
std::string getVideoID(const std::string& line){
    return getInfo(line, "\"id\":");
}


// get the music id of each video in file
std::string getMusicID(const std::string& line){
    // find starting position
    std::string keyword = "\"musicId\":";
    size_t startPos = line.find(keyword);

    // check if this position exists
    if(startPos == std::string::npos){
        return " ";
    }
    startPos+= keyword.size()+2; // skip the keyword and read its content
    size_t endPos = line.find("\"},"); // find ending position

    return line.substr(startPos, endPos - startPos); // returns music id

    
}

// get the artist name of each video in file
std::string getArtistName(const std::string& line){
    return getInfo(line, "\"musicAuthor\":");
}

// get the music name of each video in file
std::string getMusicName(const std::string& line){
    return getInfo(line, "\"musicName\":");
}


std::string getCoverMediumURL(const std::string& line){
     return getInfo(line, "\"coverMediumUrl\":");
}


// get the views of each video in file
long getVideoViews(const std::string& line){
    std::string startWord = "\"playCount\":";
    size_t startPos = line.rfind(startWord);

    //if keyword doesn't exist, return nothing
    if(startPos == std::string::npos){   
        return -1;
    }
    // keyword exists, so skip this keyword
    else{
        startPos = startPos+ startWord.size()+1 ;
    }

    // find the end of this key value pair
    std::string tmp =line.substr(startPos);
    size_t endPos = tmp.find(",");

    return std::stol(tmp.substr(0,endPos));
}


// given a line, find each hashtag with the # of times it's used, and the videos associated with it
void getHashtagInfo(const std::string& line, const Video& v){
    std::string text =  getInfo(line, "\"text\":");

    // if text is empty, return early
    if(text == " " || text == ""){
        return;
    }

    // the text of the post is given as a std::string, extract hashtags from the text.
    // define a regular expression to match hashtags
    std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)");

    // create an iterator for matching
    std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
    std::sregex_iterator endIterator;

    // iterate over the matches and extract the hashtags
    while (hashtagIterator != endIterator) {
        std::smatch match = *hashtagIterator;
        std::string hashtag = match.str(1);  // extract the first capturing group
        
	    // add to total view count of this hashtag
        hashtagInfo[hashtag].incrementTotalViews(v.getViews()); 
        hashtagInfo[hashtag].addVideo(v); // add to video associated with this hashtag

        ++hashtagIterator;
    }
    
}
// sort by video ID
bool byBiggestID(const Video& a, const Video& b){
    return a.getID() > b.getID();
}

// takes the information from map and populate it into the priority queue
void populateHashQueue(hashTrend& t, hashInfo& tInfo){
    hashInfo::iterator info_itr;
    
   // traverse through map
    for(info_itr = tInfo.begin(); info_itr != tInfo.end(); ++info_itr){
        info_itr->second.sortVideos();
        info_itr->second.sortVideosByID(byBiggestID);
        
        
        t.push(*info_itr); // insert into priority queue
    }
}

// output top 20 hashtags along w/ its 3 videos
void outputHashTo(std::ofstream& outFile, hashTrend& topTrends ){

    // variable initialization
    hashStats tmp_top;
    std::vector<Video> tmp_vid;
    int maxVidSize;
    int maxHashSize;

    if(topTrends.size() >=20){
        maxHashSize = 20;
    }
    else{
        maxHashSize = topTrends.size();
    }

    outFile<<"trending hashtags:"<<"\n";
    outFile<<"\n";

    // loop through top 20 hashtags
    for(int i = 0; i<maxHashSize; i++){

        tmp_top = topTrends.top();
        tmp_vid = tmp_top.second.getVideos();
        // check how many videos to print out (max is 3)
        if(tmp_vid.size() >=3){
            maxVidSize = 3;
        }
        else{
            maxVidSize = tmp_vid.size();
        }

        // print info of each hashtag: hashtah name, total views, video, etc
        outFile<<"========================"<<"\n";
        outFile<<"#"<<tmp_top.first<<"\n";
        outFile<<"used "<<tmp_top.second.getUsageCount()<<" "<<"times"<<"\n";
        outFile<<tmp_top.second.getTotalViews()<<" "<<"views"<<"\n";
        outFile<<"\n";

        for(int j = 0; j<maxVidSize; j++){
            outFile<<"cover url: "<<tmp_vid[j].getCoverURL()<<"\n";
            outFile<<"web video url: "<<tmp_vid[j].getWebVideoURL()<<"\n";
        }  

        outFile<<"========================"<<"\n";
        topTrends.pop(); // remove the top element & find the next hashtag to print out
    }

}

// add elements into the soundsInfo map
void getSoundInfo(const std::string& line){
    
    // variable initialization
    std::string keyword = "\"musicMeta\":";
    std::string musicMeta;
    size_t musicMetaPos = line.find(keyword);

    // check if the music meta exists
    if(musicMetaPos == std::string::npos){
        return;
    }
    // get music meta if it exsits
    else{
        musicMeta = line.substr(musicMetaPos+keyword.size());
    }

    // check if the music name can be found;
    std::string musicName = getMusicName(musicMeta);
    if(musicName == " "){
        return;
    }

    // create the music video associated w/ this sound
    Video musicVid(getVideoViews(line),
                   getCoverURL(musicMeta),
                   getWebVideoURL(musicMeta));
    
    // get the artist name, song, and music id
    musicInfo mInfo(getArtistName(musicMeta), getMusicName(musicMeta), getMusicID(musicMeta));

    // push the above information into queue
    soundsInfo[mInfo].addVideo(musicVid);
    soundsInfo[mInfo].incrementTotalViews(musicVid.getViews());
    

}

// put each element in sInfo into the priority queue
void populateSoundQueue(soundTrend& s, soundInfo& sInfo){
    soundInfo::iterator sItr;
    
    // iterate through map
    for(sItr = sInfo.begin(); sItr != sInfo.end(); ++sItr){
        sItr->second.sortVideos();
        sItr->second.sortVideosByID(byBiggestID);
        s.push(*sItr); // add each element in map into queue
        
    }
}

// output top 20 sounds into file
void outputSoundTo(std::ofstream& outFile, soundTrend& topTrends){

    // variable initialization
    soundStats tmp_top;
    std::vector<Video> tmp_vids;
    int maxVidSize;
    int maxSoundSize;

    // check output top 20 sounds or less, depending on file size
    if(topTrends.size() >=20){
        maxSoundSize = 20;
    }
    else{
        maxSoundSize = topTrends.size();
    }

    outFile<<"trending sounds:"<<"\n";
    outFile<<"\n";

    // output top 20 sounds
    for(int i = 0; i<maxSoundSize; ++i){
        tmp_top = topTrends.top();
        tmp_vids = tmp_top.second.getVideos();

        // check how many videos to output (max is 3)
        if(tmp_vids.size() >= 3){
            maxVidSize = 3;
        }
        else{
            maxVidSize = tmp_vids.size();
        }

        // output info like artist name, total views, id, etc
        outFile<<"========================"<<"\n";
        outFile<<tmp_top.first.getMusicName()<<"\n";
        outFile<<tmp_top.second.getTotalViews()<<" "<<"views"<<"\n";
        outFile<<tmp_top.first.getArtist()<<"\n";
        outFile<<"music id: "<<tmp_top.first.getId()<<"\n";
        outFile<<"\n";

        // output the top 3 videos base on views first, then by id
        for(int i = 0; i<maxVidSize; ++i){
            outFile<<"cover url: "<<tmp_vids[i].getCoverURL()<<"\n";
            outFile<<"web video url: "<<tmp_vids[i].getWebVideoURL()<<"\n";
            
        }

        outFile<<"========================"<<"\n";
        topTrends.pop();
    }
}


int main(int argc, char* argv[]){

    // check number of args
    if(argc != 4){
        std::cerr<<"Usage: nytrends.exe input.json output.txt [video/hashtag]"<<std::endl;
        exit(1);
    }


    std::ifstream inFile(argv[1]); // read from this file
    std::ofstream outFile(argv[2]); //output to this file
    std::string line;
    hashTrend topHashTrends; // stores all hastags
    soundTrend topSoundTrends; // store all sounds
 
    // check which operation is being called
    if(strcmp(argv[3],"hashtag") == 0){
        // loop through file
        while(std::getline(inFile, line)){
            // get each video and other stats
            Video v(getVideoViews(line), getCoverURL(line), getWebVideoURL(line), getVideoID(line));
            getHashtagInfo(line, v); 
        }
        populateHashQueue(topHashTrends,hashtagInfo); // push element into queue
        outputHashTo(outFile, topHashTrends); // output the hashtags
    }
    else if (strcmp(argv[3],"sound") == 0){
        // loop through file
        while(std::getline(inFile, line)){
            // get each video and other stats.
           getSoundInfo(line);
           
        }
        populateSoundQueue(topSoundTrends, soundsInfo); // push elements into queue
        outputSoundTo(outFile, topSoundTrends); // output sound
        
    }
    return 0;
}