CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include -I./include

all: warehouse ui

warehouse:
	$(CC) $(CFLAGS) -o ./bin/Warehouse ./src/Warehouse.cpp

ui:
	$(CC) $(CFLAGS) -o ./bin/WarehouseUI ./src/WarehouseUI.cpp

clean:
	rm -rf ./bin/*