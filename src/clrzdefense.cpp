#include "clrzdefense.hpp"

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
    auto modified_access = std::vector<std::vector<bool>>(dataset->keywords.size(), 
            std::vector<bool>(dataset->document_keywords.size(), false));
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::bernoulli_distribution distTrue(1 - TPR);
    std::bernoulli_distribution distFalse(FPR);
    std::vector<access_pattern> new_access_paterns(dataset->keyword_docs.size());
    for (keyword_id keyword_id = 0; keyword_id < dataset->keywords.size(); keyword_id++)
    {
        for (size_t doc: dataset->document_keywords[keyword_id])
        {
            modified_access[keyword_id][doc] = true;
        }

        for (size_t doc = 0; doc < dataset->document_keywords.size(); doc++)
        {
            modified_access[keyword_id][doc] = modified_access[keyword_id][doc] ? distTrue(gen) : distFalse(gen);
            if (modified_access[keyword_id][doc])
            {
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