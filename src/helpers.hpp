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