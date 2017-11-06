cmake_minimum_required(VERSION 3.2)
project(midterm)

set(CMAKE_CXX_STANDARD 11)

include(config/Macros.cmake)

find_package( Threads REQUIRED )  # Multithreading package required

# customize binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # separate binaries from build files

# add library directory
include_directories(../library/include)

add_subdirectory(q1_spinlock)
add_subdirectory(q2_primes)
add_subdirectory(q3_turtles)
add_subdirectory(q4_rollercoaster)
