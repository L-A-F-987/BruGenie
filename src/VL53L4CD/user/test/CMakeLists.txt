cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

project(VL53L4CD_tests VERSION 1.0.0 LANGUAGES CXX)

add_executable(VL53L4CD_test_1_executable boot_sensor_1.cpp)
add_executable(VL53L4CD_test_2_executable boot_sensor_2.cpp)

target_link_libraries(VL53L4CD_test_1_executable PRIVATE LIB_uld_driver) 
target_link_libraries(VL53L4CD_test_2_executable PRIVATE LIB_uld_driver) 

add_test(TestBootSensor_1_Test VL53L4CD_test_1_executable)
add_test(TestBootSensor_2_Test VL53L4CD_test_2_executable)