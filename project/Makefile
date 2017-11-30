CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include -I./include

all: warehouse ui robot

warehouse:
	$(CC) $(CFLAGS) -o ./bin/main ./src/main.cpp

ui:
	$(CC) $(CFLAGS) -o ./bin/ui ./src/WarehouseUI.cpp

robot:
	$(CC) $(CFLAGS) -o ./bin/robot ./src/Robot.cpp

clean:
	rm -rf ./bin/*