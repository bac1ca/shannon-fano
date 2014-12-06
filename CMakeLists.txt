cmake_minimum_required(VERSION 2.8.4)
project(shannon_fano)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/IOManager.cpp
    src/IOManager.h
    src/main.cpp
    src/ShannonFano.cpp
    src/ShannonFano.h)

add_executable(shannon_fano ${SOURCE_FILES})