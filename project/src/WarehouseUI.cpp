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

  public:
    
    WarehouseUI() : display(), memory(SHARED_MEMORY_NAME) {
      display.clear_all();
      display.set_cursor_visible(false);
    }

    void draw_warehouse() {
      static const char WALL = WALL_CHAR;
      static const char SHELF = SHELF_CHAR;

      WarehouseInfo& winfo = memory->winfo;
      RobotInfo& rinfo = memory->rinfo;

      display.clear_display();
      for (int r = 0; r < winfo.rows; r++) {
        display.set_cursor_position(YOFF + r, XOFF);
        for (int c = 0; c < winfo.cols; c++) {
          char ch = winfo.warehouse[c][r];
          if (ch == WALL_CHAR) {
            std::printf("%c", WALL);
          } else if (ch == SHELF_CHAR) {
            std::printf("%c", SHELF);
          } else {
            std::printf("%c", EMPTY_CHAR);
          }
        }
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
    // ui.draw_robots();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}