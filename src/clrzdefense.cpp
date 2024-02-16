#include "clrzdefense.hpp"
#include <iostream>


CLRZDefence::CLRZDefence(Dataset *dataset, double TPR, double FPR, size_t seed): Defense(dataset)
{
    this->TPR = TPR;
    this->FPR = FPR;
    this->seed = seed;
    this->name = "CLRZ";
}

CLRZDefence::~CLRZDefence()
{
}

std::vector<std::vector<access_pattern>> CLRZDefence::run_defense(std::vector<std::vector<size_t>>& queries)
{

    std::vector<std::vector<access_pattern>> results;
    
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::bernoulli_distribution distTrue(TPR);
    std::bernoulli_distribution distFalse(FPR);
    std::vector<access_pattern> new_access_paterns(dataset->train_keyword_docs.size());
    for (keyword_id keyword_id = 0; keyword_id < dataset->keywords.size(); keyword_id++)
    {
        auto modified_access = new bool[dataset->document_keywords.size()];
        std::fill(modified_access, modified_access + dataset->document_keywords.size(), false);
        for (size_t doc: dataset->train_keyword_docs[keyword_id])
        {
            modified_access[doc] = true;
        }

        for (size_t doc = 0; doc < dataset->document_keywords.size(); doc++)
        {
            if (modified_access[doc])
            {
                if(distTrue(gen))
                    new_access_paterns[keyword_id].push_back(doc);  
            }
            else
            {
                if(distFalse(gen))
                    new_access_paterns[keyword_id].push_back(doc);
            }
        }
    }
    

    for (size_t week = 0; week < queries.size(); week++)
    {
        std::vector<size_t> &week_queries = queries[week];
        std::vector<access_pattern> week_results;
        for (size_t query: week_queries)
        {
            week_results.push_back(new_access_paterns[query]);
        }
        results.push_back(week_results);
    }

    return results;
}