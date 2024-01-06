#include <iostream>
#include <string>
#include "manager.hpp"
#include "experiment.hpp"
#include "dataset.hpp"
#include "manager.hpp"
const std::string EXPERIMENTS_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\experiments\\";
const std::string DATASET_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\datasets";
const std::string RESULTS_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\results";

int main()
{
    // Code goes here
    Manager manager(DATASET_PATH, EXPERIMENTS_PATH);

    const bool RUN_ALPHA = false;
    const bool RUN_OFFSET = false;
    const bool RUN_QUANTITY = false;
    const bool RUN_DEFENSE = true;
    std::vector<std::function<void()>> alpha_experiments = {
        [&manager]() {manager.run_alpha_experiment(0, RESULTS_PATH, "enron", 100);},
        [&manager]() {manager.run_alpha_experiment(1, RESULTS_PATH, "enron", 500);},
        [&manager]() {manager.run_alpha_experiment(2, RESULTS_PATH, "enron", 1000);},
        [&manager]() {manager.run_alpha_experiment(3, RESULTS_PATH, "enron", 3000);}
    };

    std::vector<int> offsets{0, 5, 10, 20, 50, 100, 200};
    std::vector<std::function<void()>> offset_experiments = {
        [&manager, &offsets]() {manager.run_offset_experiment(0, RESULTS_PATH, "enron", 100, 5, offsets);},
        [&manager, &offsets]() {manager.run_offset_experiment(1, RESULTS_PATH, "enron", 500, 5, offsets);},
        [&manager, &offsets]() {manager.run_offset_experiment(2, RESULTS_PATH, "enron", 1000, 5, offsets);},
        [&manager, &offsets]() {manager.run_offset_experiment(3, RESULTS_PATH, "enron", 100, 100, offsets);},
        [&manager, &offsets]() {manager.run_offset_experiment(4, RESULTS_PATH, "enron", 500, 100, offsets);},
        [&manager, &offsets]() {manager.run_offset_experiment(5, RESULTS_PATH, "enron", 1000, 100, offsets);}
    };

    std::vector<size_t> num_queries = {5, 10, 20, 50, 100, 200};
    std::vector<std::function<void()>> quantity_experiments = {
        [&manager, &num_queries]() {manager.run_quantity_experiment(0, RESULTS_PATH, "enron", 100, num_queries);},
        [&manager, &num_queries]() {manager.run_quantity_experiment(1, RESULTS_PATH, "enron", 300, num_queries);},
        [&manager, &num_queries]() {manager.run_quantity_experiment(2, RESULTS_PATH, "enron", 600, num_queries);},
        [&manager, &num_queries]() {manager.run_quantity_experiment(3, RESULTS_PATH, "enron", 1000, num_queries);},
        [&manager, &num_queries]() {manager.run_quantity_experiment(4, RESULTS_PATH, "enron", 1500, num_queries);}
    };

    std::vector<double> FPRs = {0.1, 0.2, 0.3};
    std::vector<std::function<void()>> defense_experiments = {
        [&manager, &FPRs]() {manager.run_defense_experiment(0, RESULTS_PATH, "enron", 100, FPRs, "default");},
        [&manager, &FPRs]() {manager.run_defense_experiment(1, RESULTS_PATH, "enron", 100, FPRs, "clrz");},
    };
    std::vector<std::function<void()>> experiments;
    if(RUN_ALPHA)
        experiments.insert(experiments.end(), alpha_experiments.begin(), alpha_experiments.end());
    if(RUN_OFFSET)
        experiments.insert(experiments.end(), offset_experiments.begin(), offset_experiments.end());
    if(RUN_QUANTITY)
        experiments.insert(experiments.end(), quantity_experiments.begin(), quantity_experiments.end());
    if(RUN_DEFENSE)
        experiments.insert(experiments.end(), defense_experiments.begin(), defense_experiments.end());

    for (auto experiment : experiments)
    {
        measureExecutionTime(experiment);
        std::cout << std::endl;
    }

}


