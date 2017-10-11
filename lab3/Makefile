all: ./src/shakespeare.cpp ./src/word_count.cpp
	g++ ./src/shakespeare.cpp ./src/word_count.cpp -o ./bin/shakespeare -g -std=c++11

clean: 
	$(RM) ./bin/shakespeare

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -std=c++11

# build sources

# the build target executable:
TARGET = ./bin/shakespeare

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)