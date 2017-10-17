CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include -I/Users/jaeharys/OneDrive/UBC/4th\ Year/CPEN\ 333/labs/lab5/include

.PHONY: music test clean

music: server client
server:
	$(CC) $(CFLAGS) -o ./bin/music_library_server ./src/music_library_server.cpp

client:
	$(CC) $(CFLAGS) -o ./bin/music_library_client ./src/music_library_client.cpp

test:
	$(CC) $(CFLAGS) -o ./bin/music_library_test ./test/music_library_test.cpp -I/Users/jaeharys/OneDrive/UBC/4th\ Year/CPEN\ 333/labs/lab5/src

clean:
	rm -r ./bin/*