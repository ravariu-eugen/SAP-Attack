cmake_minimum_required(VERSION 3.20)
project(SAPAttack)

# Add source files to be compiled
file(GLOB SOURCES "*.cpp")
add_executable(SAPAttack ${SOURCES})

# Set C++ standard
set_target_properties(SAPAttack PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    )

target_include_directories(SAPAttack PUBLIC
    "${PROJECT_BINARY_DIR}"
    )