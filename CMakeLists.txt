# Project information
cmake_minimum_required(VERSION 3.14)
project(cpp-snippets
    VERSION 1.0.0
    DESCRIPTION "C++ Snippets"
    HOMEPAGE_URL "https://github.com/reftch/cpp-snippets"
)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(snippets)
