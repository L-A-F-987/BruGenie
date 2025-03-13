cmake_minimum_required(VERSION 3.10)
project(BruGenie LANGUAGES CXX)


enable_testing()

add_subdirectory(Initial_Framework)


add_library(Framework STATIC
        Initial_Framework/Framework_code/Ultrasound_Sensors.cpp
        )

target_include_directories(Framework
        PUBLIC
        header_files/)



