#ifndef LAB4_MAZE_RUNNER_COMMON_H
#define LAB4_MAZE_RUNNER_COMMON_H

#define MAZE_MEMORY_NAME "lab4_maze_runner"
#define MAZE_MUTEX_NAME "lab4_maze_runner_mutex"

#define WALL_CHAR 'X'
#define EMPTY_CHAR ' '
#define EXIT_CHAR 'E'

#define COL_IDX 0
#define ROW_IDX 1

#define MAX_MAZE_SIZE 80
#define MAX_RUNNERS   50

struct MazeInfo {
  int rows;           // rows in maze
  int cols;           // columns in maze
  char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];  // maze storage
};

struct RunnerInfo {
  int nrunners;      // number runners
  int rloc[MAX_RUNNERS][2];   // runner locations [col][row]
};

struct SharedData {
  MazeInfo minfo;    // maze info
  RunnerInfo rinfo;  // runner info
  bool quit;         // tell everyone to quit
};

#endif //LAB4_MAZE_RUNNER_COMMON_H
