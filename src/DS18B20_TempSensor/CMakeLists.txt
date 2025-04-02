cmake_minimum_required(VERSION 3.10)
project(BruGenie LANGUAGES CXX)


add_library(Lib_DS18B20
  SHARED
  ./DS18B20.cpp
)

add_library(cpptimer INTERFACE)

set_target_properties(cpptimer PROPERTIES
  PUBLIC_HEADER CppTimer.h)


add_executable(DS18B20_test DS18B20Usage.cpp)

target_link_libraries(DS18B20_test PRIVATE Lib_DS18B20)






