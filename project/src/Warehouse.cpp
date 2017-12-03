#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cpen333/process/shared_memory.h>
#include <json.hpp>
using JSON = nlohmann::json;

#include "common.h"
#include "Robot.h"
#include "Product.h"

class Computer {
  cpen333::process::shared_object<SharedData> memory_;
  std::map<std::string, int> inventory_;
  std::vector<Robot*> robots_;

 public:
  Computer() : memory_(SHARED_MEMORY_NAME), robots_(), inventory_() { }

  void load_warehouse(std::string filename) {
      memory_->winfo.rows = 0;
      memory_->winfo.cols = 0;

      std::ifstream fin(filename);
      std::string line;

      if (fin.is_open()) {
        int row = 0;

        while (std::getline(fin, line)) {
          int cols = line.length();
          if (cols > 0) {
            if (cols > memory_->winfo.cols) memory_->winfo.cols = cols;

            for (size_t col = 0; col < cols; col++) {
              memory_->winfo.warehouse[col][row] = line[col];
            }

            row++;
          }
        }

        memory_->winfo.rows = row;
        fin.close();
      }
  }

  void load_inventory() {
    std::ifstream fin("data/products.json");

    size_t nadded = 0;
    if (fin.is_open()) {
      JSON jinventory;
      fin >> jinventory;

      for (const auto& jitem : jinventory) {
        Product p;
        p.name = jitem["name"];
        p.weight = (double)jitem["weight"];
        p.quantity = 10;

        auto it = inventory_.insert({ p.name, p.quantity });
      }
    }
  }

  void init_robots() {
    memory_->rinfo.nrobots = 0;

    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> rdist(0, memory_->winfo.rows);
    std::uniform_int_distribution<size_t> cdist(0, memory_->winfo.cols);

    for (size_t i = 0; i < MAX_ROBOTS; i++) {
      size_t r, c;
      do {
        r = rdist(rnd);
        c = cdist(rnd);
      } while (memory_->winfo.warehouse[c][r] != EMPTY_CHAR);

      memory_->rinfo.rloc[i][COL_IDX] = c;
      memory_->rinfo.rloc[i][ROW_IDX] = r;
    }
  }

  void show_menu() {
    safe_printf("WAREHOUSE MANAGER, ENTER A COMMAND\n");
    safe_printf("0: Quit\n");
    safe_printf("1: Spawn Robot\n");
    safe_printf("2: View Inventory\n");
    safe_printf("\n");
    
  }

  void spawn_robot() {
    if (robots_.size() == MAX_ROBOTS) {
      safe_printf("MAX reached\n\n");
      return;
    }

    safe_printf("Spawning...\n\n");
    robots_.push_back(new Robot());
    robots_.back()->start();
  }

  void view_inventory() {
    for (const auto& pair : inventory_) {
      safe_printf("%s: %d\n", pair.first.c_str(), pair.second);
    }
    safe_printf("\n");
  }

};

int main(int argc, char* argv[]) {
  // load warehouse layout
  std::string name = "data/warehouse0.txt";
  if (argc > 1) name = argv[1];

  // initialize shared memory for visualization
  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);

  // start computer and load layout and inventory
  Computer computer;
  computer.load_warehouse(name);
  computer.load_inventory();
  computer.init_robots();

  char cmd = 0;
  while (!memory->quit) {
    computer.show_menu();

    std::cin >> cmd;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (cmd) {
      case '0':
        memory->quit = true;
        break;
      case '1':
        computer.spawn_robot();
        break;
      case '2':
        computer.view_inventory();
        break;
      default:
       safe_printf("nani?\n");
    }
  }

  memory.unlink();
  return 0;
}