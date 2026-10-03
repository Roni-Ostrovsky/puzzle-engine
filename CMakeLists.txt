cmake_minimum_required(VERSION 3.20)
project(PuzzleEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(project_settings INTERFACE)
target_include_directories(project_settings INTERFACE src)

if(MSVC)
    target_compile_options(project_settings INTERFACE /W4)
else()
    target_compile_options(project_settings INTERFACE -Wall -Wextra -Wpedantic)
    target_compile_options(project_settings INTERFACE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
    target_link_options(project_settings INTERFACE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endif()


add_executable(puzzle src/main.cpp src/Board.hpp src/rules_checking.cpp)
target_link_libraries(puzzle PRIVATE project_settings)


enable_testing()

add_executable(tests tests/test_main.cpp tests/test_board.cpp src/rules_checking.cpp)
target_link_libraries(tests PRIVATE project_settings)
target_include_directories(tests PRIVATE tests)

add_test(NAME board_tests COMMAND tests)