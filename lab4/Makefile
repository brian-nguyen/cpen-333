CC = g++
CFLAGS = -g -std=c++11 -I/Users/jaeharys/cpen333/include

.PHONY: maze malware clean

maze: parent ui runner
parent:
	$(CC) $(CFLAGS) -o ./bin/maze_runner_main ./src/maze_runner_main.cpp

ui:
	$(CC) $(CFLAGS) -o ./bin/maze_runner_ui ./src/maze_runner_ui.cpp

runner:
	$(CC) $(CFLAGS) -o ./bin/maze_runner ./src/maze_runner.cpp

malware:
	$(CC) $(CFLAGS) -o ./bin/malware ./src/malware.cpp

clean:
	rm -r ./bin/*