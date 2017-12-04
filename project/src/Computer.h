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
#include "Order.h"
#include "DynamicOrderQueue.h"

class Computer {
  cpen333::process::shared_object<SharedData> memory_;
  std::map<std::string, int> inventory_;

  DynamicOrderQueue queue_;
  std::vector<Robot*> robots_;

  int total_orders_;

 public:

  Computer() : memory_(SHARED_MEMORY_NAME), robots_(), inventory_(), queue_() { }

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
    safe_printf("WAREHOUSE MANAGER, ENTER A COMMAND\n0: Quit\n1: Spawn Robot\n2: View Inventory\n3: Add Test Orders\n4: Add Order\n");
  }

  void spawn_robot() {
    if (robot_count() == MAX_ROBOTS) {
      std::cout << "MAX REACHED\n";
      return;
    }

    robots_.push_back(new Robot(queue_));
    robots_.back()->start();
  }

  int robot_count() {
    return robots_.size();
  }

  std::vector<Robot*>& robots() {
    return robots_;
  }

  DynamicOrderQueue& queue() {
    return queue_;
  }

  void add_order() {
    total_orders_++;
    Order o(total_orders_, NEW_ORDER);

    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> rdist(0, memory_->winfo.rows);
    std::uniform_int_distribution<size_t> cdist(0, memory_->winfo.cols);

    std::vector<std::pair<int, int>> goal;
    std::pair<int, int> pt(0, 0);
    for (int i = 0; i < 5; i++) {
      do {
        pt.first = rdist(rnd);
        pt.second = cdist(rnd);
      } while (memory_->winfo.warehouse[pt.first][pt.second] != EMPTY_CHAR);
    
      goal.push_back(pt);
    }
    
    queue_.add(o);
  }

  void test_order_queue() {
      // some test orders
      Order o1(0, NEW_ORDER);
      Order o2(1, NEW_ORDER);
      Order o3(2, NEW_ORDER);
      std::pair<int, int> g1(3, 1);
      std::pair<int, int> g2(15, 1);
      std::pair<int, int> g3(5, 1);
      std::pair<int, int> g4(15, 5);
      
      std::vector<std::pair<int, int>> goals1;
      std::vector<std::pair<int, int>> goals2;
      std::vector<std::pair<int, int>> goals3;
      
      goals1.push_back(g1);
      goals1.push_back(g2);
      goals1.push_back(g3);
      goals1.push_back(g4);
      o1.set_route(goals1);
      goals2.push_back(g4);
      goals2.push_back(g3);
      goals2.push_back(g2);
      goals2.push_back(g1);
      o2.set_route(goals2);
      goals3.push_back(g3);
      goals3.push_back(g4);
      goals3.push_back(g1);
      goals3.push_back(g2);
      o3.set_route(goals3);

      queue_.add(o1);
      queue_.add(o2);
      queue_.add(o3);
      total_orders_ += 3;
  }

  void view_inventory() {
    for (const auto& pair : inventory_) {
      safe_printf("%s: %d\n", pair.first.c_str(), pair.second);
    }
    safe_printf("\n");
  }

};

#endif