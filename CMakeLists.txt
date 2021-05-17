cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable(states_counter states_counter.cpp)
add_executable(simulator simulator.cpp)

add_executable(board_tests board_tests.cpp)
add_executable(constants_tests constants_tests.cpp)
add_executable(state_tests state_tests.cpp)

add_test(board_tests board_tests)
add_test(constants_tests constants_tests)
add_test(state_tests state_tests)

target_link_libraries(board_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(constants_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(state_tests ${GTEST_LIBRARIES} pthread)