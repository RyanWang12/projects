#include <string>
#include <vector>
#include "video.h"
#include <algorithm>
#ifndef __TREND_H
#define __ TREND_H


// comparator to sort videos in each hashtag
bool byBiggestVideo(const Video& a, const Video& b){
    std::string a_id = a.getID();
    std::string b_id = b.getID();


    // sort by views
    return a.getViews() > b.getViews();
}


class TrendStat{ // hold the stats of either hashtag or sound
    public:
        // overloaded constructor
        TrendStat( const long& totalViews):
        totalViews(totalViews){}

        TrendStat():totalViews(0) {videos.reserve(5); }

        // getters
        long getUsageCount()const {return videos.size(); }
        long getTotalViews()const{return totalViews;}
        void incrementTotalViews(const int& n ){totalViews+=n; }
        void addVideo(const Video& v){videos.push_back(v);}
        std::vector<Video> getVideos() const {return videos; }

        // sort the vector of videos
        void sortVideos(){std::sort(videos.begin(), videos.end(), byBiggestVideo); }

        void sortVideosByID (bool (*comp)(const Video& , const Video&)){
            int vidSize;
            if(videos.size() < 2){
                vidSize = videos.size();
                std::sort(videos.begin(), videos.begin()+vidSize, comp);
            }
            else{
                 std::sort(videos.begin(), videos.begin()+3, comp);
            }
            }

        
    private:
        long totalViews;
        std::vector<Video> videos;
};




#endif