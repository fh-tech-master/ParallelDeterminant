cmake_minimum_required(VERSION 3.17)
project(ParallelDeterminant)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(ParallelDeterminant main.cpp)