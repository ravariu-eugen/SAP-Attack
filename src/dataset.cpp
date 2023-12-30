#include "dataset.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


Dataset::Dataset(const std::string &name, const std::string &path)
{
    this->name = name;
    const std::string dataset_folder = 
        path + '\\' + name;
    std::string line;
    // read keywords
    std::ifstream keywords_file(
        dataset_folder + '\\' + "keyword_trends.txt");
        // read number of weeks

    std::getline(keywords_file, line);
    std::stringstream ss(line);
    ss >> trend_weeks;
    while (std::getline(keywords_file, line)){

        std::stringstream ss(line);
        std::string keyword;
        size_t count;
        std::vector<double> trend_data(trend_weeks);

        ss >> keyword >> count;
        for (size_t i = 0; i < trend_weeks; i++)
        {
            double value;
            ss >> value;
            trend_data[i] = value;
        }

        keywords.push_back(keyword);
        keyword_index[keyword] = keywords.size() - 1;
        keyword_data.push_back({count, trend_data});
    }
    keyword_docs.resize(keywords.size());
    // read documents

    std::ifstream documents_file(
        dataset_folder + '\\' + "document_keywords.txt");

    while (std::getline(documents_file, line)){

        std::stringstream ss(line);
        size_t keyword_count;
        ss >> keyword_count;
        std::vector<keyword_id> doc_keywords(keyword_count);
        std::string keyword;
        for (size_t i = 0; i < keyword_count; i++){
            ss >> keyword;
            doc_keywords[i] = getKeywordID(keyword);
            keyword_docs[getKeywordID(keyword)]
                    .push_back(document_keywords.size());
        }

        document_keywords.push_back(doc_keywords);
    }

}

keyword_id Dataset::getKeywordID(std::string keyword){
    return keyword_index[keyword];
}

double Dataset::getTrendValue(keyword_id keyword_id, size_t week){
    return keyword_data[keyword_id].trend_data[week];
}

std::vector<size_t>& Dataset::getKeywordDocuments(keyword_id keyword_id){
    return keyword_docs[keyword_id];
}

Dataset::~Dataset()
{
}

std::string& Dataset::getName()
{
    return name;
}
