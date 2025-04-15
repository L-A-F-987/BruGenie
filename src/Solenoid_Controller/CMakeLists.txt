
add_library(LIB_Solenoid_Controller
  SHARED
  Solenoid_Controller.cpp
)

target_link_libraries(LIB_Solenoid_Controller LIB_Pin_Control)

add_subdirectory(test)




