#include "manager.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

Dataset* Manager::loadDataset(const std::string& name){
    std::cout << "Loading Dataset " << name << std::endl;
    if (datasets.find(name) == datasets.end())
        datasets[name] = new Dataset(name, dataset_path);
    std::cout << "Loaded Dataset " << name << std::endl;
    return datasets[name];
}


Manager::Manager(std::string dataset_path, std::string experiments_path): dataset_path(dataset_path), experiments_path(experiments_path)
{
    std::cout << "Creating Manager" << std::endl;
}

Manager::~Manager()
{
    for (auto it = datasets.begin(); it != datasets.end(); it++)
    {
        delete it->second;
    }
}
std::vector<Experiment> Manager::loadExperiments(){
    std::cout << "Loading Experiments" << std::endl;
    std::vector<Experiment> experiments;
    std::filesystem::path experiments_path(experiments_path);
    if (std::filesystem::is_directory(experiments_path))
    {
        for (const auto& entry : std::filesystem::directory_iterator(experiments_path))
        {
            std::cout << entry.path().string() << std::endl;
            experiments.push_back(read_experiment(entry.path().string()));
        }
    }
    return experiments;
}

Experiment Manager::read_experiment(const std::string& path){

    std::ifstream in(path);
    std::string name, query_dist, attack_name, defense_name, dataset_name;
    size_t seed, num_runs, num_keywords, observation_offset, obs_weeks, num_queries_per_week;
    std::map<std::string, std::string> attack_params, defense_params;
    in >> name >> seed >> num_runs;
    in >> dataset_name >> num_keywords >> observation_offset;
    Dataset* dataset = loadDataset(dataset_name);
    in >> obs_weeks >> query_dist >> num_queries_per_week;
    in >> attack_name;
    std::cout << attack_name << std::endl;
    if(attack_name == "SAPAttack"){
        std::string alpha;
        in >> alpha;
        attack_params["alpha"] = alpha;
    }
    std::cout << defense_name << std::endl;
    in >> defense_name;

    std::cout << name << std::endl;

    return Experiment::Experiment(name, seed, num_runs, 
    dataset, num_keywords, observation_offset, 
    obs_weeks, query_dist, num_queries_per_week, 
    attack_name, attack_params, 
    defense_name, defense_params);
}