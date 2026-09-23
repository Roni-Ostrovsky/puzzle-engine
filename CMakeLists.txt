cmake_minimum_required(VERSION 3.20)
project(PuzzleEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(puzzle src/main.cpp src/Board.hpp)
target_include_directories(puzzle PRIVATE src)

if(MSVC)
    target_compile_options(puzzle PRIVATE /W4)
else()
    target_compile_options(puzzle PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(puzzle PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
    target_link_options(puzzle PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endif()