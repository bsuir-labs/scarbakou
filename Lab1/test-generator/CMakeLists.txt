cmake_minimum_required(VERSION 3.5)

project(test-generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-generator main.cpp example.h)
