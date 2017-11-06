set(Q2_SOURCES src/primes_test.cpp src/primes.cpp)
set(Q2_HEADERS src/primes.h src/TestException.h)
set(Q2_FILES ${Q2_SOURCES} ${Q2_HEADERS})

add_thread_executable(q2_primes primes_test q2_primes "${Q2_FILES}")