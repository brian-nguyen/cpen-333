#include "common.h"

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cpen333/process/shared_memory.h>

void load_warehouse(const std::string& filename, WarehouseInfo& winfo) {
  winfo.rows = 0;
  winfo.cols = 0;

  std::ifstream fin(filename);
  std::string line;

  if (fin.is_open()) {
    int row = 0;

    while (std::getline(fin, line)) {
      int cols = line.length();
      if (cols > 0) {
        if (cols > winfo.cols) winfo.cols = cols;

        for (size_t col = 0; col < cols; col++) {
          winfo.warehouse[col][row] = line[col];
        }

        row++;
      }
    }

    winfo.rows = row;
    fin.close();
  }
}

void init_robots(const WarehouseInfo& winfo, RobotInfo& rinfo) {
  rinfo.nrobots = 0;

  std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<size_t> rdist(0, winfo.rows);
  std::uniform_int_distribution<size_t> cdist(0, winfo.cols);  
  for (size_t i = 0; i < MAX_ROBOTS; i++) {
    size_t r, c;
    do {
      r = rdist(rnd);
      c = cdist(rnd);
    } while ((winfo.warehouse[c][r] != EMPTY_CHAR) && (winfo.warehouse[c][r] != SHELF_CHAR));

    rinfo.rloc[i][COL_IDX] = c;
    rinfo.rloc[i][ROW_IDX] = r;
  }
}

int main(int argc, char* argv[]) {
  std::string warehouse = "data/warehouse0.txt";
  if (argc > 1) warehouse = argv[1];

  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);
  load_warehouse(warehouse, memory->winfo);
  init_robots(memory->winfo, memory->rinfo);

  std::cout << "Press ENTER to quit." << std::endl;
  std::cin.get();

  memory->quit = true;
  memory.unlink();
  return 0;
}