cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

project(VL53L4CD_tests VERSION 1.0.0 LANGUAGES CXX)

add_executable(Motor_test Motor_test.cpp)

target_link_libraries(Motor_test PRIVATE LIB_Motor_Control)