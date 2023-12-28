#pragma once

#include "attack.hpp"
#include "dataset.hpp"
#include <string>

class SAPAttack : public Attack
{
private:
    double alpha;
    Dataset* dataset;
    size_t offset;
    std::string defense;
public:
    SAPAttack(Dataset* dataset, size_t offset, double alpha, std::string defense);
    ~SAPAttack();
    std::vector<std::vector<keyword_id>> run_attack(std::vector<std::vector<access_pattern>>& query_traces);
};


