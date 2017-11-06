set(Q1_SOURCES src/main.cpp)
set(Q1_HEADERS src/SpinLock.h)
set(Q1_FILES ${Q1_SOURCES} ${Q1_HEADERS})

add_thread_executable(q1_spinlock spinlock q1_spinlock "${Q1_FILES}")