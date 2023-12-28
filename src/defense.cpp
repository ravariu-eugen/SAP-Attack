#include "defense.hpp"

Defense::Defense(Dataset *dataset)
{
    this->dataset = dataset;
}

Defense::~Defense()
{
}

std::vector<std::vector<access_pattern>> Defense::run_defense(std::vector<std::vector<size_t>>& queries)
{
    std::vector<std::vector<access_pattern>> results;

    for (size_t week = 0; week < queries.size(); week++)
    {
        std::vector<size_t> &week_queries = queries[week];
        std::vector<access_pattern> week_results;
        for (size_t query: week_queries)
        {
            week_results.push_back(dataset->getKeywordDocuments(query));
        }
        results.push_back(week_results);
    }

    return results;
}