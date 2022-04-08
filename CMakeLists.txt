cmake_minimum_required(VERSION 3.5)

project(Mix VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(PROJECT_SOURCES main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/code/)

#for out of source build  
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

add_executable(Mix ${PROJECT_SOURCES})