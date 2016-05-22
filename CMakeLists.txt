cmake_minimum_required(VERSION 3.3)
project(chip8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES src/main.cpp src/Chip8.h src/Chip8.cpp)

set(CMAKE_MODULE_PATH "/home/luka/Work/personal/cmake_modules")
add_executable(chip8 ${SOURCE_FILES})

find_package(SFML COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(chip8 ${SFML_LIBRARIES})
