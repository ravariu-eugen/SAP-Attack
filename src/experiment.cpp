#include "experiment.hpp"
#include "sapattack.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <numeric>
#include "clrzdefense.hpp"


Experiment::Experiment(
    std::string name, size_t seed, size_t num_runs,
    Dataset* dataset, size_t num_keywords, size_t observation_offset,
    size_t obs_weeks, std::string query_dist, size_t num_queries_per_week,
    std::string attack, std::map<std::string, std::string> attack_params, 
    std::string defense, std::map<std::string, std::string> defense_params)
{
    this->name = name;
    this->seed = seed;
    this->num_runs = num_runs;
    this->num_keywords = num_keywords;
    this->dataset = dataset;

    if (attack == "SAPAttack"){
        double alpha = std::stod(attack_params["alpha"]);
        std::string known_defense = attack_params["defense"];
        this->attack = new SAPAttack(dataset, observation_offset, alpha, known_defense, defense_params);
    }
    if (defense == "default"){
        this->defense = new Defense(dataset);
    }
    else if (defense == "CLRZ"){
        double TPR = std::stod(defense_params["TPR"]);
        double FPR = std::stod(defense_params["FPR"]);;
        this->defense = new CLRZDefence(dataset, TPR, FPR, seed);
    }
    this->query_dist = query_dist;
    this->obs_weeks = obs_weeks;
    this->num_queries_per_week = num_queries_per_week;
    this->observation_offset = observation_offset;
}



Experiment::~Experiment()
{
}
std::string Experiment::to_string()
{
    std::stringstream ss;
    ss << "Experiment: " << name << "\n";
    ss << "Seed: " << seed << "\n";
    ss << "Num runs: " << num_runs << "\n";
    ss << "Dataset: " << dataset->name << "\n";
    ss << "Num keywords: " << num_keywords << "\n";
    ss << "Observation offset: " << observation_offset << "\n";
    ss << "Observation weeks: " << obs_weeks << "\n";
    ss << "Query distribution: " << query_dist << "\n";
    ss << "Num queries per week: " << num_queries_per_week << "\n";
    ss << attack->to_string();
    ss << defense->to_string();

    std::string s = ss.str();
    return s;
}
std::string Experiment::getName()
{
    return name;
}
std::vector<keyword_id> Experiment::create_keyword_list(size_t seed, bool random)
{

    size_t total_keywords = dataset->keyword_data.size();
    std::vector<keyword_id> all_keywords(total_keywords);
    std::iota(all_keywords.begin(), all_keywords.end(), 0);
    if (random){
        std::mt19937 gen(static_cast<unsigned int>(seed));
        std::shuffle(all_keywords.begin(), all_keywords.end(), gen);
    }
    std::vector<keyword_id> keywords(all_keywords.begin(), all_keywords.begin() + num_keywords);
    return keywords;
}
std::vector<std::vector<size_t>> Experiment::generate_queries(size_t seed, std::vector<keyword_id> &keyword_list)
{
    // generate for each week a set of keywords to query
    std::vector<std::vector<size_t>> queries;
    std::mt19937 gen(static_cast<unsigned int>(seed));
    size_t start_week = dataset->trend_weeks - obs_weeks;
    for (size_t week  = 0; week < obs_weeks; week++){
        // for each week
        size_t num_queries;

        // pick number of queries
        if (query_dist == "uniform"){
            // uniform
            num_queries = num_queries_per_week;
        } else if (query_dist == "poisson")
        {
            // poisson
            std::poisson_distribution<> dist(static_cast<unsigned int>(num_queries_per_week));
            num_queries = dist(gen);
        }

        std::vector<size_t> week_queries(num_queries);
        
        for (int query = 0; query < num_queries; query++){
            std::vector<double> weights(keyword_list.size());
            for (size_t i = 0; i < keyword_list.size(); i++)
                weights[i] = dataset->getTrendValue(keyword_list[i], start_week + week);
            std::discrete_distribution<> d_dist(weights.begin(), weights.end());
            week_queries[query] = keyword_list[d_dist(gen)];
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
    std::vector keyword_list = create_keyword_list(seed, true);
    std::vector<std::vector<size_t>> queries = generate_queries(seed, keyword_list);

    // print queries
    //log_to_stdout(print_trace(queries, "queries"));

    // defense - turn queries into access patterns
    std::vector<std::vector<access_pattern>> observed_patterns = defense->run_defense(queries);

    // attack - match keywords to acces patterns
    auto start_time = std::chrono::high_resolution_clock::now(); 
    std::vector<std::vector<size_t>> predicted_queries = attack->run_attack(observed_patterns, keyword_list);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    float durationInSeconds = static_cast<float>(duration) / 1000000;

    // evaluate
    float accuracy = evaluate_prediction(predicted_queries, queries);

    return experimentResults{accuracy, durationInSeconds};
}

std::vector<experimentResults> Experiment::run_experiment(){


std::cout << "Running experiment: " << name << std::endl;
    float avg_accuracy = 0;
    float avg_time = 0;
    std::vector<experimentResults> results(num_runs);
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < num_runs; i++){
        results[i] = run_round(seed + i);
    }
    return results;
}

experimentResults Experiment::average(std::vector<experimentResults> results)
{
    float avg_accuracy = 0;
    float avg_time = 0;
    size_t num_runs = results.size();
    for (experimentResults result : results){
        avg_accuracy += result.accuracy;
        avg_time += result.time;
    }
    avg_accuracy /= num_runs;
    avg_time /= num_runs;
    return experimentResults{avg_accuracy, avg_time};
}
