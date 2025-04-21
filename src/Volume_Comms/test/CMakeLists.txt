


add_executable(Changing_Address_test change_address_tests.cpp)


target_link_libraries(Changing_Address_test LIB_Volume_Comms)

add_test(Test_Change_Sensor_Address Changing_Address_test)


