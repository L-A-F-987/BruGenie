
add_library(LIB_Pin_Control
  SHARED
  Pin_Control_Wrapper.cpp
)


target_link_libraries(LIB_Pin_Control gpiod)

add_subdirectory(test)



