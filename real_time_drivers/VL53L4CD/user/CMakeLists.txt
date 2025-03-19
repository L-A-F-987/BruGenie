cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)


project(VL53L4CD VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

##subdirectory containing the tests provided by the driver creators
add_subdirectory(./VL53L4CD/user/test)

##compile options to check the code 
add_compile_options(-Wall -Wconversion -Wextra -pedantic)


##creating and linking libraries


add_library(LIB_uld_driver
  SHARED
  user/uld-driver/VL53L4CD_api.cpp
)

#example files 
add_library(LIB_test_cpps
  SHARED
  user/examples/Example_1_ranging_basic.cpp
  user/examples/Example_2_low_power.cpp
  )

target_include_directories(LIB_uld_driver PUBLIC uld-driver/)
target_include_directories(LIB_test_cpps PUBLIC examples/)


target_link_libraries(LIB_uld_driver gpiod)
target_link_libraries(LIB_PLATFORM gpiod LIB_uld_driver LIB_test_cpps)



