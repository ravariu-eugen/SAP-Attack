#pragma once
#include <string>
#include <vector>
#include <map> 

#include "helpers.hpp"

class Dataset {
private:

    struct keywordData
    {

        size_t count;
        std::vector<double> trend_data;
    };

    

public:

    std::string name;
    size_t trend_weeks;
    std::vector<std::string> keywords;
    std::vector<keywordData> keyword_data;
    std::map<std::string, keyword_id> keyword_index;
    std::vector<std::vector<keyword_id>> document_keywords; 
    std::vector<std::vector<keyword_id>> keyword_docs; 
    
    
    double getTrendValue(keyword_id keyword_id, size_t week);
    keyword_id getKeywordID(std::string keyword);
    std::string& getName();
    std::vector<size_t>& getKeywordDocuments(keyword_id keyword_id);
    Dataset(const std::string &name, const std::string &path);
    ~Dataset();
};


