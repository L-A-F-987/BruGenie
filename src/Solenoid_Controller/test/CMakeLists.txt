cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

project(VL53L4CD_tests VERSION 1.0.0 LANGUAGES CXX)

add_executable(Solenoid_controller_test solenoid_controller_test.cpp)

target_link_libraries(Solenoid_controller_test PRIVATE LIB_Solenoid_Controller)