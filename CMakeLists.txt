cmake_minimum_required(VERSION 3.3)
project(chip8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES src/main.cpp src/chip8/chip8.h src/chip8/chip8.cpp)
set(SOURCE_FILES_TESTS src/chip8/chip8.h src/chip8/chip8.cpp src/Tests/tests.cpp src/Tests/catch.hpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
add_executable(chip8 ${SOURCE_FILES})
add_executable(tests ${SOURCE_FILES_TESTS})

find_package(SFML COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(chip8 ${SFML_LIBRARIES})
