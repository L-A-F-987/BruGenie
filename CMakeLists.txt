
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)


project(VL53L4CD VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

enable_testing()

##subdirectory containing the demo code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/demo)

##subdirectory containing the driver sensor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/VL53L4CD/user)

##subdirectory containing the temperature sensor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DS18B20_TempSensor)

##subdirectory containing a simple wrapper for switching on and off the solenoids
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Control_pins)

##subdirectory which performs the volume calculation and manages the two TOF sensors
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Volume_Comms)

##subdirectory containing the motor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/motor)

##subdirectory containing the solenoid code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Solenoid_Controller)

##compile options to check the code 
add_compile_options(-Wall -Wconversion -Wextra -pedantic)









