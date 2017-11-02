cmake_minimum_required(VERSION 3.2)
project(lab6)

set(CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED )  # Multithreading package required

# add library directory
include_directories(../library/include)
include_directories(include)

# copy data files
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}/Debug")

set(LAB6_HEADERS src/Order.h src/CircularOrderQueue.h
        src/DynamicOrderQueue.h src/SingleOrderQueue.h
        src/OrderQueue.h src/Chef.h src/Customer.h src/Server.h
        src/Menu.h src/safe_printf.h)

#=================================================================
#  Restauranat
#=================================================================
add_executable(lab6_restaurant src/restaurant.cpp ${LAB6_HEADERS})
target_link_libraries(lab6_restaurant Threads::Threads)