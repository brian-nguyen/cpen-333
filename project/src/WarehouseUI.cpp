#include "common.h"

#include <cpen333/process/shared_memory.h>
#include <cpen333/console.h>
#include <cstdio>
#include <thread>
#include <chrono>

class WarehouseUI {
  static const int XOFF = 2;
  static const int YOFF = 1;

  cpen333::console display;
  cpen333::process::shared_object<SharedData> memory;
  cpen333::process::mutex mutex;

  int prevloc[MAX_ROBOTS][2];

  public:
    
    WarehouseUI() : display(), memory(SHARED_MEMORY_NAME), mutex(SHARED_MUTEX_NAME) {
      display.clear_all();
      display.set_cursor_visible(false);
    }

    void draw_warehouse() {
      WarehouseInfo& winfo = memory->winfo;
      RobotInfo& rinfo = memory->rinfo;

      display.clear_display();
      for (int r = 0; r < winfo.rows; r++) {
        display.set_cursor_position(YOFF + r, XOFF);
        for (int c = 0; c < winfo.cols; c++) {
          char ch = winfo.warehouse[c][r];
          if (ch == WALL_CHAR) {
            std::printf("%c", WALL_CHAR);
          } else if (ch == DOCK_CHAR) {
            std::printf("%c", DOCK_CHAR);
          } else if (ch == SHELF_CHAR) {
            std::printf("%c", SHELF_CHAR);
          } else {
            std::printf("%c", EMPTY_CHAR);
          }
        }
      }

      fflush(stdout);
    }

    void draw_robots() {
      RobotInfo& rinfo = memory->rinfo;

      for (size_t i = 0; i < rinfo.nrobots; i++) {
        char me = 'A' + i;
        int newr;
        int newc;

        {
          std::lock_guard<decltype(mutex)> lock(mutex);
          newr = rinfo.rloc[i][ROW_IDX];
          newc = rinfo.rloc[i][COL_IDX];
        }

        if (newc != prevloc[i][COL_IDX] || newr != prevloc[i][ROW_IDX]) {
          display.set_cursor_position(YOFF + prevloc[i][ROW_IDX], XOFF + prevloc[i][COL_IDX]);
          std::printf("%c", EMPTY_CHAR);
          prevloc[i][COL_IDX] = newc;
          prevloc[i][ROW_IDX] = newr;
        }

        display.set_cursor_position(YOFF + newr, XOFF + newc);
        std::printf("%c", me);
      }

      fflush(stdout);
    }

    bool quit() {
      return memory->quit;
    }

    ~WarehouseUI() {
      display.clear_all();
      display.reset();
    }
};

int main() {
  WarehouseUI ui;
  ui.draw_warehouse();

  while (!ui.quit()) {
    ui.draw_robots();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}