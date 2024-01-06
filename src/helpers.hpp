#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<double>> matrix;
typedef std::vector<size_t> access_pattern;
typedef size_t keyword_id;
typedef size_t tag;

void log_to_stdout(const std::string &str);
std::string print_vector(std::vector<double> v, std::string name);
std::string print_vector(std::vector<int> v, std::string name);
std::string print_matrix(matrix m, std::string name);
std::string print_trace(std::vector<std::vector<size_t>> trace, std::string name);

matrix create_matrix(size_t rows, size_t columns, double value);
matrix scalar_multiplication(matrix m, double scalar);
matrix sum(matrix a, matrix &b);
matrix transpose(matrix m);

#include <functional>
#include <chrono>

// Wrapper function to measure execution time of a function with parameters
template<typename Func>
void measureExecutionTime(Func&& func)
{
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Execution time: " << duration/1000.0 << " seconds" << std::endl;
}