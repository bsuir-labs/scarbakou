cmake_minimum_required(VERSION 3.5)

project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak")

set (CMAKE_LINKER_FLAGS_DEBUG
    "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak")

add_executable(Lab2 main.cpp
    customarray.cpp
    customarray.h)
