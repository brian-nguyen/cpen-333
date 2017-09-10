cmake_minimum_required(VERSION 3.1)

add_executable(car_simulator src/car_simulator.cpp src/physics.cpp)

add_executable(drag_race src/drag_race.cpp src/Car.cpp src/physics.cpp)

add_executable(highway src/highway.cpp src/Car.cpp src/physics.cpp)