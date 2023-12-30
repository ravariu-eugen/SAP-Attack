#include "helpers.hpp"
#include <iostream>
#include <sstream>

std::string print_matrix(matrix m, std::string name = "Matrix") {

    std::stringstream ss;
    ss << name << ":" << std::endl;
    for (auto i : m) {
        for (auto j : i) {
            ss << j << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

std::string print_vector(std::vector<double> v, std::string name = "Vector") {

    std::stringstream ss;
    ss << name << ":" << std::endl;
    for (auto i : v) {
        ss << i << " ";
    }
    ss << std::endl;
    return ss.str();
}

std::string print_vector(std::vector<int> v, std::string name = "Vector") {

    std::stringstream ss;
    ss << name << ":" << std::endl;
    for (auto i : v) {
        ss << i << " ";
    }
    ss << std::endl;
    return ss.str();
}

std::string print_trace(std::vector<std::vector<size_t>> trace, std::string name = "Trace") {

    std::stringstream ss;
    ss << name << ":" << std::endl;
    for (auto i : trace) {
        for (auto j : i) {
            ss << j << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

void log_to_stdout(const std::string &str) {
    if (true)
    std::cout << str << std::endl;
}

matrix create_matrix(size_t rows, size_t columns, double value) {
    matrix m(rows, std::vector<double>(columns, value));
    return m;
    
}


matrix scalar_multiplication(matrix m, double scalar) {
    for (auto i : m) {
        for (auto j : i) {
            j *= scalar;
        }
    }
    return m;
}


matrix sum(matrix a, matrix &b) {
    if (a.size() != b.size() || a[0].size() != b[0].size()) {
        throw std::invalid_argument("Matrices must be the same size");
    }
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < a[i].size(); j++) {
            a[i][j] += b[i][j];
        }
    }
    return a;
}


matrix transpose(matrix m) {
    matrix t = create_matrix(m[0].size(), m.size(), 0);
    for (size_t i = 0; i < m.size(); i++) {
        for (size_t j = 0; j < m[i].size(); j++) {
            t[j][i] = m[i][j];
        }
    }
    return t;
}