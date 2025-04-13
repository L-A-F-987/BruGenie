cmake_minimum_required(VERSION 3.1.0)

project(LoadCellApp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(LIBSRC   ${CMAKE_SOURCE_DIR}/src/ADS1115_Weight_Sensor/rpi_ads1115/ads1115rpi.cpp)
set(LIBINCLUDE   ${CMAKE_SOURCE_DIR}/src/ADS1115_Weight_Sensor/rpi_ads1115/ads1115rpi.h)

add_library(ads1115rpi_static
  STATIC
  ${LIBSRC}
  )

  target_include_directories(ads1115rpi_static PUBLIC ${CMAKE_SOURCE_DIR}/src/ADS1115_Weight_Sensor/rpi_ads1115/)

  target_link_libraries(ads1115rpi_static gpiod)

  add_executable(ads1115_test ADS1115.cpp)


  target_link_libraries(ads1115_test PRIVATE ads1115rpi_static)