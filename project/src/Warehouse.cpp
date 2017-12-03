#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cpen333/process/shared_memory.h>

#define MAX_WAREHOUSE_SIZE 80
#define MAX_ROBOTS 5

#define SHARED_MEMORY_NAME "project_amazoom"

#include "Robot.h"

struct WarehouseInfo {
  int rows;
  int cols;
  char warehouse[MAX_WAREHOUSE_SIZE][MAX_WAREHOUSE_SIZE];
};

struct SharedData {
  WarehouseInfo winfo;
  bool quit;
};

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

int main(int argc, char* argv[]) {
  std::string warehouse = "data/warehouse0.txt";
  if (argc > 1) warehouse = argv[1];

  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);
  load_warehouse(warehouse, memory->winfo);

  std::vector<Robot*> robots;
  for (int i = 0; i < MAX_ROBOTS; i++) {
    robots.push_back(new Robot(i));
  }

  for (auto& robot : robots) {
    robot->start();
  }

  std::cout << "Press ENTER to quit." << std::endl;
  std::cin.get();

  memory->quit = true;
  memory.unlink();
  return 0;
}