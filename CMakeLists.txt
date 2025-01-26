cmake_minimum_required(VERSION 3.10)
project(EXAMPLE LANGUAGES CXX)



enable_testing()

add_subdirectory(Non_Project_Examples)


add_library(Hello_Team STATIC
        Non_Project_Examples/example_cpp_file/Hello_Team.cpp
        )

target_include_directories(Hello_Team
        PUBLIC
        eexample_header_files/)



