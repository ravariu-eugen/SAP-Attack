#include "dataset.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <random>


Dataset::Dataset(const std::string &name, const std::string &path, size_t seed)
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
    train_keyword_docs.resize(keywords.size());
    test_keyword_docs.resize(keywords.size());
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
        }

        document_keywords.push_back(doc_keywords);
    }


    std::vector<int> doc_index(document_keywords.size());
    is_train_doc.resize(document_keywords.size());
    std::iota(doc_index.begin(), doc_index.end(), 0);
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::shuffle(doc_index.begin(), doc_index.end(), gen);

    for (size_t i = 0; i < doc_index.size(); i++){

        bool is_train = i < 0.5 * doc_index.size();
        is_train_doc[doc_index[i]] = is_train;
        
        for (keyword_id keyword : document_keywords[doc_index[i]])
            if (is_train)
                train_keyword_docs[keyword].push_back(doc_index[i]);
            else
                test_keyword_docs[keyword].push_back(doc_index[i]);
        
    }


}

keyword_id Dataset::getKeywordID(std::string keyword){
    return keyword_index[keyword];
}

double Dataset::getTrendValue(keyword_id keyword_id, size_t week){
    return keyword_data[keyword_id].trend_data[week];
}


Dataset::~Dataset()
{
}

std::string& Dataset::getName()
{
    return name;
}
