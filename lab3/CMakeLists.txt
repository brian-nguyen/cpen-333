cmake_minimum_required(VERSION 3.7)
project(lab3)

set(CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED )  # Multithreading package required

#===================================================
# Word count and tests
#===================================================

add_executable(word_count_test src/word_count_test.cpp src/word_count.cpp)

#===================================================
#  Shakespeare
#===================================================

add_executable(shakespeare src/shakespeare.cpp src/word_count.cpp)
target_link_libraries(shakespeare Threads::Threads)

# copy data files
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}")

# add library directory
include_directories(../library/include)

#===================================================
#  System Logger
#===================================================

# child
add_executable(system_logger_child src/system_logger_child.cpp)
target_link_libraries(system_logger_child Threads::Threads)

# parent
add_executable(system_logger_parent src/system_logger_parent.cpp)
target_link_libraries(system_logger_parent Threads::Threads)

# parent depends on child
add_dependencies(system_logger_parent system_logger_child)

# link with unix realtime library
if(UNIX AND NOT APPLE)
    target_link_libraries(system_logger_child rt)
    target_link_libraries(system_logger_parent rt)
endif()

# set working directories
if (${CMAKE_VERSION} VERSION_GREATER "3.8.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.8.0")
    set_target_properties(system_logger_parent PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$(Configuration)")
endif()