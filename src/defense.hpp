#pragma once

#include <string>
#include "dataset.hpp"
#include "helpers.hpp"
class Defense
{
protected:
    /* data */
    std::string name;
    Dataset *dataset;
public:
    Defense(Dataset *dataset);
    ~Defense();
    virtual std::vector<std::vector<access_pattern>> run_defense(std::vector<std::vector<keyword_id>>& queries);
    std::string getName();
    std::string to_string();
};

