
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)


project(VL53L4CD VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

enable_testing()

##subdirectory containing the driver sensor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/VL53L4CD/user/test)

##subdirectory containing the temperature sensor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DS18B20_TempSensor)

##subdirectory containing the weight sensor code 
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ADS1115_Weight_Sensor)


##subdirectory containing the motor code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/motor)

##compile options to check the code 
add_compile_options(-Wall -Wconversion -Wextra -pedantic)


##creating and linking libraries

add_library(LIB_uld_driver
  SHARED
  ${CMAKE_SOURCE_DIR}/src/VL53L4CD/user/uld-driver/VL53L4CD_api.cpp

)

#example files 
add_library(LIB_test_cpps
  SHARED
  src/VL53L4CD/user/examples/Example_1_ranging_basic.cpp
  src/VL53L4CD/user/examples/Example_2_low_power.cpp
  )

target_include_directories(LIB_uld_driver PUBLIC /src/VL53L4CD/user/uld-driver/)
target_include_directories(LIB_test_cpps PUBLIC /src/VL53L4CD/user/examples/)

target_link_libraries(LIB_test_cpps LIB_uld_driver gpiod )






