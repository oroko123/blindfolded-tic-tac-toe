cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -std=c++17")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
option(CXX "enable C++ compilation" ON)
if(CXX)
    enable_language(CXX)
endif()

# Locate GTest 
find_package(GTest REQUIRED)
find_package(GUROBI REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

# find_library(GUROBI_LIBRARY NAMES gurobi PATHS /opt/gurobi912/linux64/lib NO_DEFAULT_PATH)
# set(GUROBI_LIBS ${GUROBI_LIBRARY})
add_executable(matrix_generator matrix_generator.cpp)
add_executable(simulator simulator.cpp)

add_executable(board_tests board_tests.cpp)
add_executable(constants_tests constants_tests.cpp)
add_executable(state_tests state_tests.cpp)
add_executable(serializer_tests serializer_tests.cpp)
add_executable(linear_program linear_program.cpp)


add_test(board_tests board_tests)
add_test(constants_tests constants_tests)
add_test(state_tests state_tests)
add_test(serializer_tests serializer_tests)

target_link_libraries(board_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(constants_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(state_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(serializer_tests ${GTEST_LIBRARIES} pthread)
if(CXX)
    set(CMAKE_CXX_STANDARD 17)
    target_link_libraries(linear_program optimized ${GUROBI_CXX_LIBRARY} stdc++fs debug ${GUROBI_CXX_DEBUG_LIBRARY})
endif()
target_link_libraries(linear_program ${GUROBI_LIBRARY})