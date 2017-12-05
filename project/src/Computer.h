#ifndef AMAZOOM_COMPUTER_H
#define AMAZOOM_COMPUTER_H

#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>

#include <json.hpp>
using JSON = nlohmann::json;

#include "common.h"
#include "Robot.h"
#include "Product.h"
#include "Shelf.h"
#include "Order.h"
#include "DynamicOrderQueue.h"

class Computer {
  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;
  std::map<std::string, int> inventory_;

  std::vector<Order> orders_;

  DynamicOrderQueue queue_;
  std::vector<Robot*> robots_;

  std::vector<Shelf> shelves_;

 public:

  Computer() :
    memory_(SHARED_MEMORY_NAME), mutex_(SHARED_MUTEX_NAME), robots_(), inventory_(), queue_(), shelves_() { }

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

      std::vector<Product> products;
      for (const auto& jitem : jinventory) {
        Product p(jitem["name"], (double)jitem["weight"], 10);
        auto it = inventory_.insert({ p.name_, p.quantity_ });
        products.push_back(p);
      }

      load_shelves(products);
    }
  }

  void load_shelves(std::vector<Product> products) {
    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> dist(0, shelves_.size());

    for (Product& p : products) {
      shelves_[dist(rnd)].add(p);
    }
  }

  void init_shelves_and_docks() {
    for (int c = 0; c < memory_->winfo.cols; c++) {
      for (int r = 0; r < memory_->winfo.rows; r++) {
        if (memory_->winfo.warehouse[c][r] == SHELF_CHAR) {
          std::pair<int, int> loc(c, r);
          Shelf s(999.0, loc);
          shelves_.push_back(s);
        } else if (memory_->winfo.warehouse[c][r] == DOCK_CHAR) {
          safe_printf("Dock located at {%d, %d}\n", c, r);
        }
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

  void spawn_robot() {
    if (robot_count() == MAX_ROBOTS) {
      std::cout << "MAX REACHED\n";
      return;
    }

    robots_.push_back(new Robot(orders_, queue_));
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

  void add_order(Order o) {
    {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      orders_.push_back(o);
    }
    queue_.add(o);
  }

  void test_order_queue() {
      // some test placed orders
      Order o1(0, PLACED);
      Order o2(1, PLACED);
      Order o3(2, PLACED);
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

      add_order(o1);
      add_order(o2);
      add_order(o3);
  }

  void show_menu() {
    safe_printf("\n---\nWAREHOUSE MANAGER, ENTER A COMMAND\n0: Quit\n1: Spawn Robot\n2: View Inventory\n3: View Stock\n4: Show Orders\n5: View Shelf\n6: Test Orders\n---\n\n");
  }

  int view_stock(std::string& name) {
    auto it = inventory_.find(name);
    if (it != inventory_.end()) {
      return it->second;
    }

    return -1;
  }

  void view_inventory() {
    for (const auto& pair : inventory_) {
      safe_printf("%s: %d\n", pair.first.c_str(), pair.second);
    }
    safe_printf("\n");
  }

  void view_shelves() {
    for (int i = 0; i < shelves_.size(); i++) {
      safe_printf("Shelf %d contains:\n", i);
      for (Product& p : shelves_[i].products()) {
        safe_printf("\t%s: %d\n", p.name_.c_str(), p.quantity_);
      }
      safe_printf("\n");
    }
  }

  void view_orders(int status, bool all = false) {
    int s = status - 48;
    if (s < 0) return;

    switch (s) {
      case PLACED:
        safe_printf("Showing placed orders\n");
        break;
      case READY:
        safe_printf("Showing ready orders\n");
        break;
      case DELIVERED:
        safe_printf("Showing delivered orders\n");
        break;
      default:
        all = true;
        safe_printf("Unknown status (%d). Showing all orders\n", status);
    }

    for (Order& o : orders_) {
      if (all || (o.status_ == s)) {
        safe_printf("Order %d\n", o.id_);
      }
    }
    safe_printf("\n");
  }

};

#endif