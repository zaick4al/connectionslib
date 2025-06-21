cmake_minimum_required(VERSION 3.13)

set(LIB_NAME ConnectionsLib)
project(${LIB_NAME} VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${LIB_NAME} STATIC
    src/cobject.h
    src/cobject.cpp
)

target_include_directories(${LIB_NAME} PUBLIC src/)