CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include
Q1DIR = ./q1_spinlock
Q2DIR = ./q2_primes
Q3DIR = ./q3_turtles
Q4DIR = ./q4_rollercoaster

q1:
	$(CC) $(CFLAGS) -o $(Q1DIR)/bin/main $(Q1DIR)/src/main.cpp

q2:
	$(CC) $(CFLAGS) -o $(Q2DIR)/bin/primes_test $(Q2DIR)/src/primes_test.cpp $(Q2DIR)/src/primes.cpp

q3: leo don mic rap shr

leo:
	$(CC) $(CFLAGS) -o $(Q3DIR)/bin/leonardo $(Q3DIR)/src/leonardo.cpp

don:
	$(CC) $(CFLAGS) -o $(Q3DIR)/bin/donatello $(Q3DIR)/src/donatello.cpp

mic:
	$(CC) $(CFLAGS) -o $(Q3DIR)/bin/michelangelo $(Q3DIR)/src/michelangelo.cpp

rap:
	$(CC) $(CFLAGS) -o $(Q3DIR)/bin/raphael $(Q3DIR)/src/raphael.cpp

shr:
	$(CC) $(CFLAGS) -o $(Q3DIR)/bin/shredder $(Q3DIR)/src/shredder.cpp

q4: rollercoaster passenger launcher unlinker

rollercoaster:
	$(CC) $(CFLAGS) -o $(Q4DIR)/bin/rollercoaster $(Q4DIR)/src/rollercoaster.cpp

passenger:
	$(CC) $(CFLAGS) -o $(Q4DIR)/bin/passenger $(Q4DIR)/src/passenger.cpp

launcher:
	$(CC) $(CFLAGS) -o $(Q4DIR)/bin/launcher $(Q4DIR)/src/launcher.cpp

unlinker:
	$(CC) $(CFLAGS) -o $(Q4DIR)/bin/unlinker $(Q4DIR)/src/unlinker.cpp

clean:
	rm -rf ./q1_spinlock/bin ./q2_primes/bin ./q3_turtles/bin ./q4_rollercoaster/bin