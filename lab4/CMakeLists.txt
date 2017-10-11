cmake_minimum_required(VERSION 3.2)
project(lab4)

set(CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED )  # Multithreading package required

# add library directory
include_directories(../library/include)

# copy data files
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}/Debug")

#===================================================
#  Maze Runner
#===================================================

# child
add_executable(maze_runner_ui src/maze_runner_ui.cpp)
target_link_libraries(maze_runner_ui Threads::Threads)

# parent
add_executable(maze_runner_main src/maze_runner_main.cpp)
target_link_libraries(maze_runner_main Threads::Threads)

# parent
add_executable(maze_runner src/maze_runner.cpp)
target_link_libraries(maze_runner Threads::Threads)

# link with unix realtime library
if(UNIX AND NOT APPLE)
    target_link_libraries(maze_runner_main rt)
    target_link_libraries(maze_runner rt)
    target_link_libraries(maze_runner_ui rt)
endif()

#===================================================
# Malware
#===================================================

add_executable(malware src/malware.cpp)
target_link_libraries(malware Threads::Threads)

# link with unix realtime library
if(UNIX AND NOT APPLE)
    target_link_libraries(malware rt)
endif()

# set working directories
if (${CMAKE_VERSION} VERSION_GREATER "3.8.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.8.0")
    set_target_properties(malware PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$(CONFIGURATION)")
    set_target_properties(maze_runner_main PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$(CONFIGURATION)")
endif()

