cmake_minimum_required(VERSION 3.20)

# Find the OpenMP package
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -openmp:llvm")
add_library(Hungarian Hungarian.cpp)


if (OpenMP_CXX_FOUND)
    target_link_libraries(Hungarian PUBLIC OpenMP::OpenMP_CXX)
endif ()