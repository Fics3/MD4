cmake_minimum_required(VERSION 3.25)
project(MD4)

set(CMAKE_CXX_STANDARD 17)

add_executable(MD4 main.cpp MD4.cpp MD4.h experiment.cpp experiment.h)
