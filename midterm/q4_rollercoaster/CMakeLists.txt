#===================================================================
# ROLLERCOASTER
#===================================================================

set(Q4_ROLLERCOASTER_ROLLERCOASTER_SOURCES src/rollercoaster.cpp)
set(Q4_ROLLERCOASTER_ROLLERCOASTER_HEADERS src/common.h)
set(Q4_ROLLERCOASTER_ROLLERCOASTER_FILES 
        ${Q4_ROLLERCOASTER_ROLLERCOASTER_SOURCES} 
        ${Q4_ROLLERCOASTER_ROLLERCOASTER_HEADERS})
add_process_executable(q4_rollercoaster_rollercoaster rollercoaster 
        q4_rollercoaster "${Q4_ROLLERCOASTER_ROLLERCOASTER_FILES}")

#===================================================================
# PASSENGER
#===================================================================

set(Q4_ROLLERCOASTER_PASSENGER_SOURCES src/passenger.cpp)
set(Q4_ROLLERCOASTER_PASSENGER_HEADERS src/common.h)
set(Q4_ROLLERCOASTER_PASSENGER_FILES 
        ${Q4_ROLLERCOASTER_PASSENGER_SOURCES} 
        ${Q4_ROLLERCOASTER_PASSENGER_HEADERS})
add_process_executable(q4_rollercoaster_passenger passenger
        q4_rollercoaster "${Q4_ROLLERCOASTER_PASSENGER_FILES}")

#===================================================================
# LAUNCHER
#===================================================================

set(Q4_ROLLERCOASTER_LAUNCHER_SOURCES src/launcher.cpp)
set(Q4_ROLLERCOASTER_LAUNCHER_HEADERS src/common.h)
set(Q4_ROLLERCOASTER_LAUNCHER_FILES
        ${Q4_ROLLERCOASTER_LAUNCHER_SOURCES}
        ${Q4_ROLLERCOASTER_LAUNCHER_HEADERS})
add_process_executable(q4_rollercoaster_launcher launcher
        q4_rollercoaster "${Q4_ROLLERCOASTER_LAUNCHER_FILES}")

add_dependencies(q4_rollercoaster_launcher
        q4_rollercoaster_rollercoaster q4_rollercoaster_passenger)