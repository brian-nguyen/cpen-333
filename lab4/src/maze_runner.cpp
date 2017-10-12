#include "maze_runner_common.h"

#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cstring>
#include <chrono>
#include <thread>

#define TAKEN '*'

class MazeRunner {

  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;

  // local copy of maze
  MazeInfo minfo_;

  // runner info
  size_t idx_;   // runner index
  int loc_[2];   // current location

 public:

  MazeRunner() : memory_(MAZE_MEMORY_NAME), mutex_(MAZE_MUTEX_NAME),
                 minfo_(), idx_(0), loc_() {

    // copy maze contents
    minfo_ = memory_->minfo;

    {
      // protect access of number of runners
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      idx_ = memory_->rinfo.nrunners;
      memory_->rinfo.nrunners++;
    }

    // get current location
    loc_[COL_IDX] = memory_->rinfo.rloc[idx_][COL_IDX];
    loc_[ROW_IDX] = memory_->rinfo.rloc[idx_][ROW_IDX];

  }

  /**
   * Solves the maze, taking time between each step so we can actually see progress in the UI
   * @return 1 for success, 0 for failure, -1 to quit
   */
  int go() {
    if (memory_->quit) return -1;

    int c = loc_[COL_IDX];
    int r = loc_[ROW_IDX];

    // update UI
    memory_->rinfo.rloc[idx_][COL_IDX] = c;
    memory_->rinfo.rloc[idx_][ROW_IDX] = r;

    // success if exit reached
    if (minfo_.maze[c][r] == EXIT_CHAR) return 1;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // mark current position
    minfo_.maze[c][r] = TAKEN;

    // go right, if this results in a solution, return success
    if (c < minfo_.cols && (minfo_.maze[c + 1][r] == EMPTY_CHAR || minfo_.maze[c + 1][r] == EXIT_CHAR)) {
      loc_[COL_IDX] = c + 1;
      loc_[ROW_IDX] = r;
      if (go() == 1) return 1;
    }

    // go left
    if (c >= 0 && (minfo_.maze[c - 1][r] == EMPTY_CHAR || minfo_.maze[c - 1][r] == EXIT_CHAR)) {
      loc_[COL_IDX] = c - 1;
      loc_[ROW_IDX] = r;
      if (go() == 1) return 1;
    }

    // go down
    if (r < minfo_.rows && (minfo_.maze[c][r + 1] == EMPTY_CHAR || minfo_.maze[c][r + 1] == EXIT_CHAR)) {
      loc_[COL_IDX] = c;
      loc_[ROW_IDX] = r + 1;
      if (go() == 1) return 1;
    }

    // go up
    if (r >= 0 && (minfo_.maze[c][r - 1] == EMPTY_CHAR || minfo_.maze[c][r - 1] == EXIT_CHAR)) {
      loc_[COL_IDX] = c;
      loc_[ROW_IDX] = r - 1;
      if (go() == 1) return 1;
    }

    // no solution found, backtrack
    minfo_.maze[c][r] = EMPTY_CHAR;

    // update UI
    memory_->rinfo.rloc[idx_][COL_IDX] = c;
    memory_->rinfo.rloc[idx_][ROW_IDX] = r;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return memory_->quit ? -1 : 0;
  }
};

int main() {

  MazeRunner runner;
  std::printf("Result: %d\n", runner.go());

  return 0;
}