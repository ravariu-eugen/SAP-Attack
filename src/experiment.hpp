#pragma once

#include <string>
#include <map>
#include "dataset.hpp"
#include "attack.hpp"
#include "defense.hpp"
#include <random>
#include "helpers.hpp"
struct experimentResults{
    float accuracy;
    float time;
};
class Experiment
{
private:
    /* data */
    std::string name;
    size_t seed;
    size_t num_runs;

    Dataset* dataset;
    size_t num_keywords;
    size_t observation_offset;

    std::string query_dist;
    size_t obs_weeks;
    size_t num_queries_per_week;
    
    Attack* attack;
    Defense* defense;
    
    std::vector<std::vector<size_t>> generate_queries(size_t seed);
    experimentResults run_round(size_t seed);
public:
    experimentResults run_experiment();
    Experiment read_experiment(const std::string& path);
    Experiment(
    std::string name, size_t seed, size_t num_runs,
    Dataset* dataset, size_t num_keywords, size_t observation_offset,
    size_t obs_weeks, std::string query_dist, size_t num_queries_per_week,
    std::string attack, std::map<std::string, std::string> attack_params, 
    std::string defense, std::map<std::string, std::string> defense_params);
    ~Experiment();
};




