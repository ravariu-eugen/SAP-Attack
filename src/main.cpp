#include <iostream>
#include <string>
#include "manager.hpp"
#include "experiment.hpp"
#include "dataset.hpp"
#include <filesystem>
#include <fstream>
const std::string EXPERIMENTS_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\experiments\\";
const std::string DATASET_PATH = "E:\\Facultate\\anul4\\IC\\proiect c++\\datasets";

std::vector<Experiment> loadExperiments(){
    for (const auto & entry : std::filesystem::directory_iterator(EXPERIMENTS_PATH))
    {
        std::cout << entry.path() << std::endl;
    }
}
int main()
{
    // Code goes here
    
    

    Dataset dataset = Dataset("enron", DATASET_PATH);

    std::cout << "Finished loading dataset " << dataset.getName() << std::endl;
    std::map<std::string, Dataset> datasets;
    // load experiment
    return 0;
}

