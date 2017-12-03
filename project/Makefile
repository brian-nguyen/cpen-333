CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include -I./include

all: warehouse ui

warehouse:
	$(CC) $(CFLAGS) -o ./bin/Warehouse ./src/Warehouse.cpp

ui:
	$(CC) $(CFLAGS) -o ./bin/WarehouseUI ./src/WarehouseUI.cpp

order_test:
	$(CC) $(CFLAGS) -o ./bin/tests/Order_test ./tests/Order_test.cpp -I./src

shelf_test:
	$(CC) $(CFLAGS) -o ./bin/tests/Shelf_test ./tests/Shelf_test.cpp -I./src

clean:
	rm -rf ./bin/*