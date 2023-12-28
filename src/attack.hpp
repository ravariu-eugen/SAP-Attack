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
    std::vector<std::vector<tag>> process_traces(std::vector<std::vector<access_pattern>>& observed_patterns);
    virtual std::vector<std::vector<size_t>> run_attack(std::vector<std::vector<access_pattern>>& observed_patterns);
};


