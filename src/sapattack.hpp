#pragma once

#include "attack.hpp"
#include "dataset.hpp"
#include <string>
#include "Hungarian.h"

class SAPAttack : public Attack
{
private:
    double alpha;
    Dataset* dataset;
    size_t offset;
    std::string defense;
    std::map<std::string, std::string> defense_params;
public:
    SAPAttack(Dataset* dataset, size_t offset, double alpha, std::string defense = "default", 
                std::map<std::string, std::string> defense_params = {});
    ~SAPAttack();
    std::vector<std::vector<keyword_id>> run_attack(std::vector<std::vector<access_pattern>>& query_traces, 
                                                    std::vector<keyword_id> &selected_keywords);
    std::string to_string();
};


