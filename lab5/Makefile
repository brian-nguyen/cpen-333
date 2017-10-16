CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include -I/Users/jaeharys/OneDrive/UBC/4th\ Year/CPEN\ 333/labs/lab5/include -I/Users/jaeharys/OneDrive/UBC/4th\ Year/CPEN\ 333/labs/lab5/src
DEPS = ./src/MusicLibrary.h ./src/MusicLibraryApi.h ./src/JsonConverter.h ./srcJsonMusicLibrary.h ./src/Message.h ./src/Song.h

.PHONY: music clean

music_library_test:
	$(CC) $(CFLAGS) -o ./bin/music_library_test ./test/music_library_test.cpp

clean:
	rm -r ./bin/*