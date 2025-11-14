cmake_minimum_required(VERSION 3.10)

# Project name
project(PalubeckisGen)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Add executable
add_executable(palubeckis_gen main_GEN.c gener6.c)