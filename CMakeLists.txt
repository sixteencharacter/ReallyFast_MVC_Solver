cmake_minimum_required(VERSION 3.19)
project(VertexCoverage CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenMP REQUIRED)

if (OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found, with flags: ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "OpenMP not found. Please install the correct development package.")
endif()

add_executable(runner main.cpp)
target_link_libraries(runner PUBLIC OpenMP::OpenMP_CXX)