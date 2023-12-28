#include "sapattack.hpp"

SAPAttack::SAPAttack(Dataset* dataset, size_t offset, double alpha, std::string defense = "default")
{
    this->name = "SAPAttack";
    this->alpha = alpha;
    this->dataset = dataset;
    this->defense = defense;
    this->offset = offset;
}

SAPAttack::~SAPAttack()
{
}

std::vector<std::vector<keyword_id>> SAPAttack::run_attack(std::vector<std::vector<access_pattern>>& query_traces){

    std::vector<std::vector<tag>> observed_tags = process_traces(query_traces);
    std::vector<std::vector<keyword_id>> predicted_query;
    for (auto week_tags : observed_tags){
        predicted_query.push_back(std::vector<keyword_id>(week_tags.size(), 0));
    }
    return predicted_query;
}