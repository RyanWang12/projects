#ifndef __MUSIC_H
#define __MUSIC_H
#include<string>

class musicInfo{

    public:
        // overloaded constructor
        musicInfo(const std::string& artist, const std::string& musicName, const std::string& id):
                id(id), artist(artist), musicName(musicName){}
        
        // default constructor 
        musicInfo(): id(""), artist(""), musicName(""){}

        // getters
        std::string getArtist() const {return artist;}
        std::string getId()const{return id;}
        std::string getMusicName() const {return musicName;}

        // operator== to compare hashes in priority queue
        bool operator==(const musicInfo& other) const { // operator to compare id
        return id == other.id;
    }
    private:
        // variables
        std::string id;
        std::string artist;
        std::string musicName;
    

};

// hash function to has each video id
namespace std{
    template<>
    struct hash<musicInfo> {
        std::size_t operator()(const musicInfo& m) const {
            
            return std::hash<std::string>()(m.getId()); // returns a hash from id
        }
    };
}


#endif