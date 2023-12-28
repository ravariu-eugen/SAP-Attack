#include "experiment.hpp"
#include "sapattack.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
Experiment::Experiment(
    std::string name, size_t seed, size_t num_runs,
    Dataset* dataset, size_t num_keywords, size_t observation_offset,
    size_t obs_weeks, std::string query_dist, size_t num_queries_per_week,
    std::string attack, std::map<std::string, std::string> attack_params, 
    std::string defense, std::map<std::string, std::string> defense_params)
{
    this->seed = seed;
    this->num_keywords = num_keywords;
    this->dataset = dataset;

    if (attack == "SAPAttack"){
        double alpha = std::stod(attack_params["alpha"]);
        this->attack = new SAPAttack(dataset, observation_offset, alpha, defense);
    }
    if (defense == "default"){
        this->defense = new Defense(dataset);
    }
    this->query_dist = query_dist;
    this->obs_weeks = obs_weeks;
    this->num_queries_per_week = num_queries_per_week;
    this->observation_offset = observation_offset;
}



Experiment::~Experiment()
{
}
std::vector<std::vector<size_t>> Experiment::generate_queries(size_t seed){
    // generate for each week a set of keywords to query
    std::vector<std::vector<size_t>> queries;
    std::mt19937 gen(seed);
    size_t start_week = dataset->trend_weeks - obs_weeks;
    for (size_t week  = start_week; week < start_week + obs_weeks; week++){
        // for each week
        size_t num_queries;

        // pick number of queries
        if (query_dist == "uniform"){
            // uniform
            num_queries = num_queries_per_week;
        } else if (query_dist == "poisson")
        {
            // poisson
            std::poisson_distribution<> dist(num_queries_per_week);
            num_queries = dist(gen);
        }

        std::vector<size_t> week_queries;
        
        for (int query = 0; query < num_queries; query++){
            std::vector<double> weights;
            for (int keyword = 0; keyword < num_keywords; keyword++){
                weights.push_back(
                    dataset->getTrendValue(keyword, + week)
                );
            }
            std::discrete_distribution<> d_dist(weights.begin(), weights.end());
            size_t keyword_id = d_dist(gen);
            week_queries.push_back(query);
        }
        queries.push_back(week_queries);
    }
    return queries;
}


float evaluate_prediction(std::vector<std::vector<keyword_id>> &predicted_queries, 
                          std::vector<std::vector<keyword_id>> &true_queries){
    size_t correct_queries = 0;
    size_t total_queries = 0;
    for (size_t week = 0; week < true_queries.size(); week++){
        std::vector<size_t> &true_week_queries = true_queries[week];
        std::vector<size_t> &predicted_week_queries = predicted_queries[week];
        total_queries += true_week_queries.size();
        for (size_t query = 0; query < true_week_queries.size(); query++){
            if (true_week_queries[query] == predicted_week_queries[query]){
                correct_queries++;
            }
        }
    }
    return correct_queries / (float)total_queries;
}

experimentResults Experiment::run_round(size_t seed)
{
    // generate queries
    std::vector<std::vector<size_t>> queries = generate_queries(seed);
    // defense - turn queries into access patterns
    std::vector<std::vector<access_pattern>> obeserved_patterns = defense->run_defense(queries);
    // attack - match keywords to acces patterns
    auto start_time = std::chrono::high_resolution_clock::now(); 
    std::vector<std::vector<size_t>> predicted_queries = attack->run_attack(obeserved_patterns);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    float durationInSeconds = static_cast<float>(duration) / 1000000;

    // evaluate
    float accuracy = evaluate_prediction(predicted_queries, queries);

    return experimentResults{accuracy, durationInSeconds};
}

experimentResults Experiment::run_experiment(){
    for (size_t i = 0; i < num_runs; i++){
        experimentResults result = run_round(seed + i);
        std::cout << result.accuracy << " " << result.time << std::endl;
    }
    return experimentResults{0, 0};
}