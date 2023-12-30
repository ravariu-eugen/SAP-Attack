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

    std::string name; // name of the dataset
    size_t trend_weeks; // number of weeks in the trend
    std::vector<std::string> keywords; // list of keywords
    std::vector<keywordData> keyword_data; // list of keyword data
    std::map<std::string, keyword_id> keyword_index; // map from keyword to keyword_id
    std::vector<std::vector<keyword_id>> document_keywords;  // list of keyword ids for each document
    std::vector<std::vector<keyword_id>> keyword_docs; // list of document ids for each keyword
    
    
    double getTrendValue(keyword_id keyword_id, size_t week);
    keyword_id getKeywordID(std::string keyword);
    std::string& getName();
    std::vector<size_t>& getKeywordDocuments(keyword_id keyword_id);
    Dataset(const std::string &name, const std::string &path);
    ~Dataset();
};


