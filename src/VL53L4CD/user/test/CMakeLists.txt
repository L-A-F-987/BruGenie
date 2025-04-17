cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

project(VL53L4CD_tests VERSION 1.0.0 LANGUAGES CXX)

add_executable(VL53L4CD_test_executable menu.cpp)

target_link_libraries(VL53L4CD_test_executable SHARED LIB_test_cpps LIB_uld_driver) 