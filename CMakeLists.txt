
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)


project(VL53L4CD VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

##subdirectory containing the tests provided by the driver creators
add_subdirectory(${CMAKE_SOURCE_DIR}/real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/test)

##compile options to check the code 
add_compile_options(-Wall -Wconversion -Wextra -pedantic)


##creating and linking libraries

add_library(LIB_uld_driver
  SHARED
  ${CMAKE_SOURCE_DIR}/real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/uld-driver/VL53L4CD_api.cpp

)

#example files 
add_library(LIB_test_cpps
  SHARED
  real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/examples/Example_1_ranging_basic.cpp
  real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/examples/Example_2_low_power.cpp
  )

target_include_directories(LIB_uld_driver PUBLIC /real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/uld-driver/)
target_include_directories(LIB_test_cpps PUBLIC /real_time_drivers/VL53L4CD/VL53L4CD_LinuxDriver_V2.3.2/user/examples/)

target_link_libraries( LIB_test_cpps LIB_uld_driver gpiod )






