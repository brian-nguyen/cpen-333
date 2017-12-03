#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <cpen333/process/shared_memory.h>

#include "common.h"
#include "Robot.h"

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
    } while (winfo.warehouse[c][r] != EMPTY_CHAR);

    rinfo.rloc[i][COL_IDX] = c;
    rinfo.rloc[i][ROW_IDX] = r;
  }
}

int main(int argc, char* argv[]) {
  // load warehouse layout
  std::string warehouse = "data/warehouse0.txt";
  if (argc > 1) warehouse = argv[1];

  // initialize shared memory for visualization
  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);
  load_warehouse(warehouse, memory->winfo);
  init_robots(memory->winfo, memory->rinfo);

  // start with two robots
  std::vector<Robot*> robots;
  for (int i = 0; i < 1; i++) {
    robots.push_back(new Robot());
  }

  for (auto& robot : robots) {
    robot->start();
  }


  // controls
  char cmd = 0;
  while (!memory->quit) {
    safe_printf("PICK AN OPTION\n0: Quit\n1: Add Robot\n\n");

    std::cin >> cmd;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (cmd) {
      case '0':
        memory->quit = true;
        break;
      case '1':
        robots.push_back(new Robot());
        robots.back()->start();
        break;
      default:
       safe_printf("nani?\n");
    }
  }

  // for (auto& robot : robots) {
  //   robot->join();
  // }

  memory.unlink();
  return 0;
}