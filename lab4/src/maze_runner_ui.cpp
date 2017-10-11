#include "maze_runner_common.h"

#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/console.h>
#include <cstdio>
#include <thread>
#include <chrono>

/**
 * Handles all drawing/memory synchronization for the
 * User Interface process
 * ====================================================
 *  TODO: ADD ANY NECESSARY MUTUAL EXCLUSION
 * ====================================================
 *
 */
class MazeUI {
  // display offset for better visibility
  static const int XOFF = 2;
  static const int YOFF = 1;

  cpen333::console display_;
  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;

  // previous positions of runners
  int lastpos_[MAX_RUNNERS][2];
  int exit_[2];   // exit location

 public:

  MazeUI() : display_(), memory_(MAZE_MEMORY_NAME), mutex_(MAZE_MUTEX_NAME){

    // clear display and hide cursor
    display_.clear_all();
    display_.set_cursor_visible(false);

    // initialize last known runner positions
    for (size_t i=0; i<MAX_RUNNERS; ++i) {
      lastpos_[i][COL_IDX] = -1;
      lastpos_[i][ROW_IDX] = -1;
    }

    // initialize exit location
    exit_[COL_IDX] = -1;
    exit_[ROW_IDX] = -1;

    //===========================================================
    // TODO: SEARCH MAZE FOR EXIT LOCATION
    //===========================================================

  }

  /**
   * Draws the maze itself
   */
  void draw_maze() {
    static const char WALL = 219;  // WALL character, or change to 'X' if trouble printing
    static const char EXIT = 176;  // EXIT character, or change to 'E' if trouble printing

    MazeInfo& minfo = memory_->minfo;
    RunnerInfo& rinfo = memory_->rinfo;

    // clear display
    display_.clear_display();

    // draw maze
    for (int r = 0; r < minfo.rows; ++r) {
      display_.set_cursor_position(YOFF+r, XOFF);
      for (int c = 0; c < minfo.cols; ++c) {
        char ch = minfo.maze[c][r];
        if (ch == WALL_CHAR) {
          std::printf("%c", WALL);
        } else if (ch == EXIT_CHAR){
          std::printf("%c", EXIT);
        } else {
          std::printf("%c", EMPTY_CHAR);
        }
      }
    }
    fflush(stdout);  // force output buffer to flush 
  }

  /**
   * Draws all runners in the maze
   */
  void draw_runners() {

    RunnerInfo& rinfo = memory_->rinfo;

    // draw all runner locations
    for (size_t i=0; i<rinfo.nrunners; ++i) {
      char me = 'A'+i;
      int newr = rinfo.rloc[i][ROW_IDX];
      int newc = rinfo.rloc[i][COL_IDX];

      // if not already at the exit...
      if (newc != exit_[COL_IDX] || newr != exit_[ROW_IDX]) {
        if (newc != lastpos_[i][COL_IDX]
            || newr != lastpos_[i][ROW_IDX]) {

          // zero out last spot and update known location
          display_.set_cursor_position(YOFF+lastpos_[i][ROW_IDX], XOFF+lastpos_[i][COL_IDX]);
          std::printf("%c", EMPTY_CHAR);
          lastpos_[i][COL_IDX] = newc;
          lastpos_[i][ROW_IDX] = newr;
        }

        // print runner at new location
        display_.set_cursor_position(YOFF+newr, XOFF+newc);
        std::printf("%c", me);
      } else {

        // erase old position if now finished
        if (lastpos_[i][COL_IDX] != exit_[COL_IDX] || lastpos_[i][ROW_IDX] != exit_[ROW_IDX]) {
          display_.set_cursor_position(YOFF+lastpos_[i][ROW_IDX], XOFF+lastpos_[i][COL_IDX]);
          std::printf("%c", EMPTY_CHAR);
          lastpos_[i][COL_IDX] = newc;
          lastpos_[i][ROW_IDX] = newr;

          // display a completion message
          display_.set_cursor_position(YOFF, XOFF+memory_->minfo.cols+2);
          std::printf("runner %c escaped!!", me);
        }
      }
    }
    fflush(stdout);  // force output buffer to flush
    
  }

  /**
   * Checks if we are supposed to quit
   * @return true if memory tells us to quit
   */
  bool quit() {
    // check if we need to quit
    return memory_->quit;
  }

  ~MazeUI(){
    // reset console settings
    display_.clear_all();
    display_.reset();
  }
};

int main() {

  // initialize previous locations of characters
  MazeUI ui;
  ui.draw_maze();

  // continue looping until main program has quit
  while(!ui.quit()) {
    ui.draw_runners();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
