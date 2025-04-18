cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

add_library(LIB_Volume_Comms
  SHARED
  Volume_Comms.cpp
)



target_link_libraries(LIB_Volume_Comms LIB_Pin_Control LIB_uld_driver LIB_Motor_Control)

add_subdirectory(test)