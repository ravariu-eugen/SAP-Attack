#pragma once

#include <string>
#include <vector>
#include "helpers.hpp"

class Attack
{

private:

    /* data */
    
protected:
    std::string name = "default";
    std::vector<access_pattern> tags;
public:
    Attack();
    ~Attack();
    tag getTag(access_pattern access_pattern);
    std::vector<std::vector<tag>> process_traces(std::vector<std::vector<access_pattern>>& observed_patterns);
    virtual std::vector<std::vector<keyword_id>> run_attack(std::vector<std::vector<access_pattern>>& observed_patterns, std::vector<keyword_id> &selected_keywords) = 0;
    std::string getName();
    std::string to_string();
};


