#include "attack.hpp"
#include <algorithm>

Attack::Attack()
{
    
}

Attack::~Attack()
{
}

std::vector<std::vector<tag>> Attack::process_traces(std::vector<std::vector<access_pattern>>& observed_patterns){
    for(auto week: observed_patterns){
        for(auto query: week){
            if (std::find(tags.begin(), tags.end(), query) == tags.end()){
                tags.push_back(query);
            }
        }
    }
}