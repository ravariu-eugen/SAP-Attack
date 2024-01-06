#include "manager.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

Dataset* Manager::loadDataset(const std::string& name){
    if (datasets.find(name) == datasets.end()){
        datasets[name] = new Dataset(name, dataset_path);
    }

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
            if (entry.path().extension() == ".txt")
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
    attack_params["defense"] = "default";
    in >> name >> seed >> num_runs;
    in >> dataset_name >> num_keywords;
    Dataset* dataset = loadDataset(dataset_name);
    in >> obs_weeks >> observation_offset;
    in >> query_dist >> num_queries_per_week;
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


void Manager::run_alpha_experiment(size_t seed, std::string results_path, std::string dataset_name, size_t num_keywords, double alpha_step){
    // file to write results
    std::string folder = results_path + "\\alpha_" + dataset_name;
    std::filesystem::create_directory(folder);
    std::string file = folder + "\\" + "alpha_" + std::to_string(num_keywords) + ".txt";
    std::ofstream out(file);

    std::cout << "Running Alpha Experiment " << num_keywords << std::endl;
    
    // experiment parameters
    
    Dataset* dataset = loadDataset(dataset_name);
    size_t observation_offset = 5;
    size_t num_queries_per_week = 5;

    std::string attack = "SAPAttack";
    std::map<std::string, std::string> attack_params;
    attack_params["defense"] = "default";
    std::string defense = "default";
    std::map<std::string, std::string> defense_params;


    out << "Alpha experiment" << std::endl;
    out << num_keywords << " " << num_runs << std::endl;


    for(double alpha = 0.0; alpha < 1.0; alpha += alpha_step){
        std::string name = "alpha_" + std::to_string(alpha);
        attack_params["alpha"] = std::to_string(alpha);
        Experiment exp(name, seed, num_runs, 
                    dataset, num_keywords, observation_offset, 
                    obs_weeks, query_dist, num_queries_per_week, 
                    attack, attack_params,
                    defense, defense_params);

        std::vector<experimentResults> results = exp.run_experiment();             
        out << alpha << " ";
        for (auto it = results.begin(); it != results.end(); it++)
        {
            out << it->accuracy << " ";
        }
        out << std::endl;
    
    }

}


void Manager::run_offset_experiment(size_t seed, std::string results_path, std::string dataset_name, size_t num_keywords, size_t num_queries_per_week, std::vector<int> &offsets){

    // file to write results
    std::string folder = results_path + "\\offset_" + dataset_name;
    std::filesystem::create_directory(folder);
    std::string file = folder + "\\" + "offset_" + std::to_string(num_keywords)
                                         + "_" + std::to_string(num_queries_per_week) + ".txt";
    std::ofstream out(file);

    std::cout << "Running Offset Experiment " << num_keywords << std::endl;

    // experiment parameters
    
    Dataset* dataset = loadDataset(dataset_name);

    std::string attack = "SAPAttack";
    std::map<std::string, std::string> attack_params;
    attack_params["alpha"] = "0.5";
    attack_params["defense"] = "default";
    std::string defense = "default";
    std::map<std::string, std::string> defense_params;


    out << "Offset experiment" << std::endl;
    out << num_keywords << " " << num_queries_per_week << " " << num_runs << std::endl;


    for(int offset : offsets){
        std::string name = "offset_" + std::to_string(offset);
        Experiment exp(name, seed, num_runs, 
                    dataset, num_keywords, offset, 
                    obs_weeks, query_dist, num_queries_per_week, 
                    attack, attack_params,
                    defense, defense_params);

        std::vector<experimentResults> results = exp.run_experiment();

        out << offset << " ";
        for (auto it = results.begin(); it != results.end(); it++)
        {
            out << it->accuracy << " ";
        }
        out << std::endl;

    }
}

void Manager::run_quantity_experiment(size_t seed, std::string results_path, std::string dataset_name, size_t num_keywords, std::vector<size_t> num_queries){

    // file to write results
    std::string folder = results_path + "\\quantity_" + dataset_name;
    std::filesystem::create_directory(folder);
    std::string file = folder + "\\" + "quantity_" + std::to_string(num_keywords) + ".txt";
    std::ofstream out(file);

    std::cout << "Running Quantity Experiment " << num_keywords << std::endl;

    // experiment parameters
    std::string attack = "SAPAttack";
    std::map<std::string, std::string> attack_params;
    attack_params["alpha"] = "0.5";
    attack_params["defense"] = "default";
    std::string defense = "default";
    std::map<std::string, std::string> defense_params;

    Dataset* dataset = loadDataset(dataset_name);
    size_t observation_offset = 5;


    out << "Quantity experiment" << std::endl;
    out << num_keywords << " " << num_runs << std::endl;

    for (size_t num_queries_per_week : num_queries)
    {
        std::string name = "quantity_" + std::to_string(num_queries_per_week);
        Experiment exp(name, seed, num_runs, 
                    dataset, num_keywords, observation_offset, 
                    obs_weeks, query_dist, num_queries_per_week, 
                    attack, attack_params,
                    defense, defense_params);
        std::vector<experimentResults> results = exp.run_experiment();
        out << num_queries_per_week << " ";
        for (auto it = results.begin(); it != results.end(); it++)
        {
            out << it->accuracy << " ";
        }
        out << std::endl;
        for (auto it = results.begin(); it != results.end(); it++)
        {
            out << it->time << " ";
        }
        out << std::endl;
    }

}

void Manager::run_defense_experiment(size_t seed, std::string results_path, std::string dataset_name, 
        size_t num_keywords, std::vector<double> FPRs, std::string known_defense)
{

    // file to write results
    std::string folder = results_path + "\\defense_" + dataset_name;
    std::filesystem::create_directory(folder);
    std::string file = folder + "\\" + "defense_" + std::to_string(num_keywords) + ".txt";
    std::ofstream out(file);

    std::cout << "Running Defense Experiment " << num_keywords << std::endl;
    
    // experiment parameters
    std::string attack = "SAPAttack";
    std::map<std::string, std::string> attack_params;
    attack_params["alpha"] = "0.5";
    attack_params["defense"] = known_defense;
    std::string defense = "CLRZ";
    std::map<std::string, std::string> defense_params;
    defense_params["TPR"] = "0.999";

    Dataset* dataset = loadDataset(dataset_name);
    size_t observation_offset = 5;
    size_t num_queries_per_week = 5;

    for (double FPR : FPRs)
    {
        defense_params["FPR"] = std::to_string(FPR);
        std::string name = "defense_" + std::to_string(FPR);
        Experiment exp(name, seed, num_runs, 
                    dataset, num_keywords, observation_offset, 
                    obs_weeks, query_dist, num_queries_per_week, 
                    attack, attack_params,
                    defense, defense_params);
        std::vector<experimentResults> results = exp.run_experiment();
        out << FPR << " ";
        for (auto it = results.begin(); it != results.end(); it++)
        {
            out << it->accuracy << " ";
        }
        out << std::endl;
    }
}
