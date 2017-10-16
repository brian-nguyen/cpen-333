cmake_minimum_required(VERSION 3.2)
project(lab5)

set(CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED )  # Multithreading package required

# add library directory
include_directories(../library/include)
# add json support
include_directories(include)
# access to src/ files for tests
include_directories(src)

# copy data files
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}/Debug")

#===================================================
#  Music Library
#===================================================

set(MUSIC_LIBRARY_HEADERS
        src/MusicLibrary.h
        src/MusicLibraryApi.h
        src/JsonConverter.h
        src/JsonMusicLibraryApi.h
        src/Message.h
        src/Song.h)

# tests
add_executable(music_library_test test/music_library_test.cpp ${MUSIC_LIBRARY_HEADERS})

# server
add_executable(music_library_server src/music_library_server.cpp ${MUSIC_LIBRARY_HEADERS})
target_link_libraries(music_library_server Threads::Threads)

# client
add_executable(music_library_client src/music_library_client.cpp ${MUSIC_LIBRARY_HEADERS})
target_link_libraries(music_library_client Threads::Threads)

# link with unix realtime library
if(UNIX AND NOT APPLE)
    target_link_libraries(music_library_server rt)
    target_link_libraries(music_library_client rt)
endif()

if(WIN32)
    target_link_libraries(music_library_client wsock32 ws2_32)
    target_link_libraries(music_library_server wsock32 ws2_32)
endif()