cmake_minimum_required(VERSION 3.10)
# 'Google_test' is the subproject name
project(EXAMPLE_CMAKE LANGUAGES CXX)


add_executable(EXAMPLE_executable
        Example.cpp
)

target_link_libraries(EXAMPLE_executable PRIVATE Hello_Team)