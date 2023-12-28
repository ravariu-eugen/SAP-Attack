#include "manager.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

Dataset* Manager::loadDataset(const std::string& name){
    if (datasets.find(name) == datasets.end())
        datasets[name] = new Dataset(name, dataset_path);
    
    return datasets[name];
}


Manager::Manager(std::string dataset_path, std::string experiments_path): dataset_path(dataset_path), experiments_path(experiments_path)
{
}

Manager::~Manager()
{
    for (auto it = datasets.begin(); it != datasets.end(); it++)
    {
        delete it->second;
    }
}
std::vector<Experiment> Manager::loadExperiments(){
    std::vector<Experiment> experiments;
    for (const auto & entry : std::filesystem::directory_iterator(experiments_path))
    {
        std::cout << entry.path() << std::endl;
        experiments.push_back(read_experiment(entry.path().c_str()));
    }
}

Experiment Manager::read_experiment(const std::string& path){

    std::ifstream in(path);
    std::string name, query_dist, attack_name, defense_name, dataset_name;
    size_t seed, num_runs, num_keywords, observation_offset, obs_weeks, num_queries_per_week;
    std::map<std::string, std::string> attack_params, defense_params;
    in >> name >> seed >> num_runs;
    in >> dataset_name >> num_keywords >> observation_offset;
    Dataset* dataset = new Dataset(dataset_name, path);
    in >> obs_weeks >> query_dist >> num_queries_per_week;
    in >> attack_name;
    if(attack_name == "SAPAttack"){
        std::string alpha;
        in >> alpha;
        attack_params["alpha"] = alpha;
    }
    in >> defense_name;
    return Experiment::Experiment(name, seed, num_runs, 
    dataset, num_keywords, observation_offset, 
    obs_weeks, query_dist, num_queries_per_week, 
    attack_name, attack_params, 
    defense_name, defense_params);
}