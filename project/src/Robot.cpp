#include "common.h"

#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cstring>
#include <chrono>
#include <thread>

#define TAKEN '*'

class Robot {
  cpen333::process::shared_object<SharedData> memory;
  cpen333::process::mutex mutex;
  WarehouseInfo winfo;
  size_t id;
  int loc[2];

  public:
    Robot() : memory(SHARED_MEMORY_NAME), mutex(SHARED_MUTEX_NAME), winfo(), id(0), loc() {
      winfo = memory->winfo;
      {
        std::lock_guard<decltype(mutex)> lock(mutex);
        id = memory->rinfo.nrobots;
        memory->rinfo.nrobots++;
      }

      loc[COL_IDX] = memory->rinfo.rloc[id][COL_IDX];
      loc[ROW_IDX] = memory->rinfo.rloc[id][ROW_IDX];      
    }

    int retrieve() {
      int c = loc[COL_IDX];
      int r = loc[ROW_IDX];

      if ((winfo.warehouse[c][r] == WALL_CHAR) || (winfo.warehouse[c][r] == SHELF_CHAR) || (winfo.warehouse[c][r] == DOCK_CHAR)) {
        return 0;
      }

      memory->rinfo.rloc[id][COL_IDX] = c;
      memory->rinfo.rloc[id][ROW_IDX] = r;

      // winfo.warehouse[c][r] = TAKEN;

      // go right
      if (c < winfo.cols - 1) {
        loc[COL_IDX] = c + 1;
        loc[ROW_IDX] = r;
        if (retrieve() == 1) return 1;
      }

      // go left
      if (c > 0) {
        loc[COL_IDX] = c - 1;
        loc[ROW_IDX] = r;
        if (retrieve() == 1) return 1;
      }

      // go down
      if (r < winfo.rows - 1) {
        loc[COL_IDX] = c;
        loc[ROW_IDX] = r + 1;
        if (retrieve() == 1) return 1;        
      }

      // go up
      if (r > 0) {
        loc[COL_IDX] = c;
        loc[ROW_IDX] = r - 1;
        if (retrieve() == 1) return 1;        
      }

      // winfo.warehouse[c][r] = EMPTY_CHAR;

      memory->rinfo.rloc[id][COL_IDX] = c;
      memory->rinfo.rloc[id][ROW_IDX] = r;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      return 0;
    }
};

int main() {
  Robot wall_e;
  wall_e.retrieve();
  return 0;
}