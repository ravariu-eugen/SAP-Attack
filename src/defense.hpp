#pragma once

#include <string>
#include "dataset.hpp"
#include "helpers.hpp"
class Defense
{
protected:
    /* data */
    std::string name = "default";
    Dataset *dataset;
public:
    Defense(Dataset *dataset);
    ~Defense();
    std::vector<std::vector<access_pattern>> run_defense(std::vector<std::vector<keyword_id>>& queries);
};
