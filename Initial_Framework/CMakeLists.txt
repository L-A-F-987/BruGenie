cmake_minimum_required(VERSION 3.10)



project(BruGenie LANGUAGES CXX)

enable_testing()

add_executable(volume_calc_test volume_calc_test.cpp)

target_link_libraries(volume_calc_test PRIVATE Framework)

add_test(TestVolume_Calculation volume_calc_test)


