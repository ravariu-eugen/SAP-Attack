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
public:
    Manager(std::string dataset_path, std::string experiments_path);
    std::vector<Experiment> loadExperiments();
    Experiment read_experiment(const std::string& path);
    Dataset* loadDataset(const std::string& name);
    ~Manager();
};

