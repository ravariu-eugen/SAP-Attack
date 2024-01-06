#include "attack.hpp"
#include <algorithm>

Attack::Attack()
{
    
}

Attack::~Attack()
{
}

tag Attack::getTag(access_pattern &pattern, std::vector<access_pattern> &tags){
    auto it = std::find(tags.begin(), tags.end(), pattern);
    if (it == tags.end()){
        tags.push_back(pattern);
        return tags.size() - 1;
    }
    return std::distance(tags.begin(), it);
}
std::pair<std::vector<std::vector<tag>>, std::vector<access_pattern>> Attack::process_traces(std::vector<std::vector<access_pattern>>& observed_patterns){
    std::vector<std::vector<tag>> tag_traces;
    std::vector<access_pattern> tags;
    for(auto week: observed_patterns){
        std::vector<tag> week_tags;
        for(auto access_pattern: week){
            week_tags.push_back(getTag(access_pattern, tags));
        }
        tag_traces.push_back(week_tags);
    }
    return {tag_traces, tags};
}

std::string Attack::getName()
{
    return name;
}

std::string Attack::to_string(){
    std::string s = "Attack: " + name;
    return s;
}