#ifndef AMAZOOM_COMPUTER_H
#define AMAZOOM_COMPUTER_H

#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>

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
        Product p(jitem["name"], (double)jitem["weight"], 10);

        auto it = inventory_.insert({ p.name_, p.quantity_ });
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
    if (robot_count() == MAX_ROBOTS) {
      safe_printf("MAX reached\n\n");
      return;
    }

    robots_.push_back(new Robot());
    robots_.back()->start();
  }

  int robot_count() {
    return robots_.size();
  }

  std::vector<Robot*>& robots() {
    return robots_;
  }

  void view_inventory() {
    for (const auto& pair : inventory_) {
      safe_printf("%s: %d\n", pair.first.c_str(), pair.second);
    }
    safe_printf("\n");
  }

};

#endif