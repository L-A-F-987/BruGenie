
add_library(LIB_Solenoid_Controller
  SHARED
  Pin_Control_Wrapper.cpp
)


target_link_libraries(LIB_Solenoid_Controller LIB_Pin_Control gpiod)




