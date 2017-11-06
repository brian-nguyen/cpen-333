#===================================================================
# Leonardo
#===================================================================

set(Q3_SHARED_MEMORY_LEONARDO_SOURCES src/leonardo.cpp)
set(Q3_SHARED_MEMORY_LEONARDO_HEADERS src/turtles.h)
set(Q3_SHARED_MEMORY_LEONARDO_FILES
        ${Q3_SHARED_MEMORY_LEONARDO_SOURCES}
        ${Q3_SHARED_MEMORY_LEONARDO_HEADERS})
add_process_executable(q3_shared_memory_leonardo leonardo
        q3_shared_memory "${Q3_SHARED_MEMORY_LEONARDO_FILES}")
add_dependencies(q3_shared_memory_leonardo q3_shared_memory_donatello)

#===================================================================
# Donatello
#===================================================================

set(Q3_SHARED_MEMORY_DONATELLO_SOURCES src/donatello.cpp)
set(Q3_SHARED_MEMORY_DONATELLO_HEADERS src/turtles.h)
set(Q3_SHARED_MEMORY_DONATELLO_FILES 
        ${Q3_SHARED_MEMORY_DONATELLO_SOURCES} 
        ${Q3_SHARED_MEMORY_DONATELLO_HEADERS})
add_process_executable(q3_shared_memory_donatello donatello 
        q3_shared_memory "${Q3_SHARED_MEMORY_DONATELLO_FILES}")

add_dependencies(q3_shared_memory_donatello q3_shared_memory_michelangelo)

#===================================================================
# Michelangelo
#===================================================================

set(Q3_SHARED_MEMORY_MICHELANGELO_SOURCES src/michelangelo.cpp)
set(Q3_SHARED_MEMORY_MICHELANGELO_HEADERS src/turtles.h)
set(Q3_SHARED_MEMORY_MICHELANGELO_FILES
        ${Q3_SHARED_MEMORY_MICHELANGELO_SOURCES}
        ${Q3_SHARED_MEMORY_MICHELANGELO_HEADERS})
add_process_executable(q3_shared_memory_michelangelo michelangelo
        q3_shared_memory "${Q3_SHARED_MEMORY_MICHELANGELO_FILES}")

add_dependencies(q3_shared_memory_michelangelo q3_shared_memory_raphael)

#===================================================================
# Raphael
#===================================================================

set(Q3_SHARED_MEMORY_RAPHAEL_SOURCES src/raphael.cpp)
set(Q3_SHARED_MEMORY_RAPHAEL_HEADERS src/turtles.h)
set(Q3_SHARED_MEMORY_RAPHAEL_FILES
        ${Q3_SHARED_MEMORY_RAPHAEL_SOURCES}
        ${Q3_SHARED_MEMORY_RAPHAEL_HEADERS})
add_process_executable(q3_shared_memory_raphael raphael
        q3_shared_memory "${Q3_SHARED_MEMORY_RAPHAEL_FILES}")

# Removed due to circular dependency issue, run leo first
# add_dependencies(q3_shared_memory_raphael q3_shared_memory_leonardo)

#===================================================================
# Shredder
#===================================================================

set(Q3_SHARED_MEMORY_SHREDDER_SOURCES src/shredder.cpp)
set(Q3_SHARED_MEMORY_SHREDDER_HEADERS src/turtles.h)
set(Q3_SHARED_MEMORY_SHREDDER_FILES
        ${Q3_SHARED_MEMORY_SHREDDER_SOURCES}
        ${Q3_SHARED_MEMORY_SHREDDER_HEADERS})
add_process_executable(q3_shared_memory_shredder shredder
        q3_shared_memory "${Q3_SHARED_MEMORY_SHREDDER_FILES}")