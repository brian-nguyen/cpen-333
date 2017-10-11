#include "maze_runner_common.h"

#include <string>
#include <fstream>
#include <thread>
#include <random>
#include <cpen333/process/shared_memory.h>

/**
 * Reads a maze from a filename and populates the maze
 * @param filename file to load maze from
 * @param minfo maze info to populate
 */
void load_maze(const std::string& filename, MazeInfo& minfo) {

  // initialize number of rows and columns
  minfo.rows = 0;
  minfo.cols = 0;

  std::ifstream fin(filename);
  std::string line;

  // read maze file
  if (fin.is_open()) {
    int row = 0;  // zeroeth row
    while (std::getline(fin, line)) {
      int cols = line.length();
      if (cols > 0) {
        // longest row defines columns
        if (cols > minfo.cols) {
          minfo.cols = cols;
        }
        for (size_t col=0; col<cols; ++col) {
          minfo.maze[col][row] = line[col];
        }
        ++row;
      }
    }
    minfo.rows = row;
    fin.close();
  }

}

/**
 * Randomly places all possible maze runners on an empty
 * square in the maze
 * @param minfo maze input
 * @param rinfo runner info to populate
 */
void init_runners(const MazeInfo& minfo, RunnerInfo& rinfo) {
  rinfo.nrunners = 0;

  // fill in random placements for future runners
  std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<size_t> rdist(0, minfo.rows);
  std::uniform_int_distribution<size_t> cdist(0, minfo.cols);
  for (size_t i=0; i<MAX_RUNNERS; ++i) {
    // generate until on an empty space
    size_t r,c;
    do {
      r = rdist(rnd);
      c = cdist(rnd);
    } while (minfo.maze[c][r] != EMPTY_CHAR);
    rinfo.rloc[i][COL_IDX] = c;
    rinfo.rloc[i][ROW_IDX] = r;
  }
}

int main(int argc, char* argv[]) {

  // read maze from command-line, default to maze0
  std::string maze = "data/maze0.txt";
  if (argc > 1) {
    maze = argv[1];
  }

  //===============================================================
  //  TODO:  CREATE SHARED MEMORY AND INITIALIZE IT
  //===============================================================
  cpen333::process::shared_object<SharedData> memory("lab4_maze_runner");
  load_maze(maze, memory->minfo);
  init_runners(memory->minfo, memory->rinfo);

  std::cout << "Keep this running until you are done with the program." << std::endl << std::endl;
  std::cout << "Press ENTER to quit." << std::endl;
  std::cin.get();

  //===============================================================
  //  TODO:  INFORM OTHER PROCESSES TO QUIT
  //===============================================================
  return 0;
}