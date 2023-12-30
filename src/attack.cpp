#include "attack.hpp"
#include <algorithm>

Attack::Attack()
{
    
}

Attack::~Attack()
{
}

tag Attack::getTag(access_pattern access_pattern){
    auto it = std::find(tags.begin(), tags.end(), access_pattern);
    if (it == tags.end()){
        tags.push_back(access_pattern);
        return tags.size() - 1;
    }
    return std::distance(tags.begin(), it);
}
std::vector<std::vector<tag>> Attack::process_traces(std::vector<std::vector<access_pattern>>& observed_patterns){
    tags.clear();
    std::vector<std::vector<tag>> tag_traces;
    for(auto week: observed_patterns){
        std::vector<tag> week_tags;
        for(auto access_pattern: week){
            week_tags.push_back(getTag(access_pattern));
        }
        tag_traces.push_back(week_tags);
    }
    return tag_traces;
}

std::string Attack::getName()
{
    return name;
}

std::string Attack::to_string(){
    std::string s = "Attack: " + name;
    return s;
}