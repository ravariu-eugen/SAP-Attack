#include <iostream>
#include <string>
#include "manager.hpp"
#include "experiment.hpp"
#include "dataset.hpp"
#include "manager.hpp"
const std::string EXPERIMENTS_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\experiments\\";
const std::string DATASET_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\datasets";


int main()
{
    // Code goes here
    Manager manager(DATASET_PATH, EXPERIMENTS_PATH);

    std::vector<Experiment> experiments = manager.loadExperiments();

    for (Experiment experiment : experiments)
    {
        std::cout << "Running experiment: " << experiment.getName() << std::endl;
        std::cout << experiment.to_string() << std::endl;
        auto results = experiment.run_experiment();
        std::cout << results.accuracy << " " << results.time << std::endl;
    }

}


