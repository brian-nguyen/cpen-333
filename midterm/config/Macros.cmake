find_package( Threads REQUIRED )  # Multithreading package required

#define a macro to simplify adding tests, then use it
macro (warnall)
    # Highest warning level
    if(MSVC)
        # Force to always compile with W4
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        # Update if necessary
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
    endif()
endmacro (warnall)

# Add an executable linking to the threads and RT library
macro(add_process_executable target output_name output_directory sources)
    add_executable(${target} ${sources})
    target_link_libraries(${target} Threads::Threads)    # link with threads library
    # shared_memory requires linking with the rt library
    if(UNIX AND NOT APPLE)
        target_link_libraries(${target} rt)
    endif()
    set_target_properties(${target} PROPERTIES
            OUTPUT_NAME ${output_name}
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${output_directory}")

    # auto-set working directory for process executables
    if (${CMAKE_VERSION} VERSION_GREATER "3.8.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.8.0")
        set_target_properties(${target} PROPERTIES
                VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${output_directory}/\$(Configuration)")
    endif()
endmacro(add_process_executable)

# Add an executable linking to the threads library
macro(add_thread_executable target output_name output_directory sources)
    add_executable(${target} ${sources})
    target_link_libraries(${target} Threads::Threads)    # link with threads library
    set_target_properties(${target} PROPERTIES
            OUTPUT_NAME ${output_name}
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${output_directory}")

    # auto-set working directory for thread executables
    if (${CMAKE_VERSION} VERSION_GREATER "3.8.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.8.0")
        set_target_properties(${target} PROPERTIES
                VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${output_directory}/\$(Configuration)")
    endif()
endmacro(add_thread_executable)
