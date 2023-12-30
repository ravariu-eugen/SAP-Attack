cmake_minimum_required(VERSION 3.20)
project(SAPAttack)


# Find the OpenMP package
find_package(OpenMP REQUIRED)

# Add source files to be compiled
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
add_subdirectory(hungarian)


file(GLOB SOURCES "*.cpp")
add_executable(SAPAttack ${SOURCES})
target_link_libraries(SAPAttack PUBLIC Hungarian)

if (OpenMP_CXX_FOUND)
    target_link_libraries(SAPAttack PUBLIC OpenMP::OpenMP_CXX)
endif ()

# Set C++ standard
set_target_properties(SAPAttack PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    )

target_include_directories(SAPAttack PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/hungarian"
    )