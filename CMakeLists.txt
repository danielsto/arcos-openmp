cmake_minimum_required(VERSION 3.6)
project(ARCOS_OpenMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(paralela Paralela/ARCfmtut_par.cpp)
add_executable(secuencial Secuencial/ARCfmtut_seq.cpp)