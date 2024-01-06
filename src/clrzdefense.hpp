#include "dataset.hpp"
#include "helpers.hpp"
#include "defense.hpp"
#include <random>
class CLRZDefence: public Defense
{
private:
    double TPR;
    double FPR;
    size_t seed;
public:

    CLRZDefence(Dataset *dataset, double TPR, double FPR, size_t seed);
    std::vector<std::vector<access_pattern>> run_defense(std::vector<std::vector<keyword_id>>& queries);
    ~CLRZDefence();
};

