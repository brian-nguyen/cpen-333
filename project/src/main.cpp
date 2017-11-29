#include "common.h"

#include <iostream>
#include <fstream>
#include <string>
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

int main(int argc, char* argv[]) {
  std::string warehouse = "data/warehouse0.txt";
  if (argc > 1) warehouse = argv[1];

  cpen333::process::shared_object<SharedData> memory("project_amazoom");
  load_warehouse(warehouse, memory->winfo);

  std::cout << "Press ENTER to quit." << std::endl;
  std::cin.get();

  memory->quit = true;
  memory.unlink();
  return 0;
}