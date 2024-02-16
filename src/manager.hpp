#pragma once
#include <vector>
#include "dataset.hpp"
#include "experiment.hpp"
class Manager
{
private:
    std::string dataset_path;
    std::string experiments_path;
    std::map <std::string, Dataset*> datasets;
    const size_t num_runs = 12;
    const size_t obs_weeks = 50;
    const std::string query_dist = "poisson";
public:
    Manager(std::string dataset_path, std::string experiments_path);
    ~Manager();

    Dataset* loadDataset(const std::string& name, size_t seed);

    void run_alpha_experiment(size_t seed, std::string results_path, std::string dataset_name, 
                            size_t num_keywords, double alpha_step = 0.1);
    void run_offset_experiment(size_t seed, std::string results_path, std::string dataset_name, 
                            size_t num_keywords, size_t num_queries_per_week, std::vector<int> &offsets);
    void run_quantity_experiment(size_t seed, std::string results_path, std::string dataset_name,
                            size_t num_keywords, std::vector<size_t> num_queries);
    void run_defense_experiment(size_t seed, std::string results_path, std::string dataset_name,
                            size_t num_keywords, std::vector<double> FPRs, std::string known_defense);
    std::vector<Experiment> loadExperiments();
    Experiment read_experiment(const std::string& path);
    std::vector<Experiment> read_experiment_series(const std::string& path);
};

