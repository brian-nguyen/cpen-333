#ifndef AMAZOOM_COMPUTER_H
#define AMAZOOM_COMPUTER_H

#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/semaphore.h>

#include <json.hpp>
using JSON = nlohmann::json;

#include "common.h"
#include "Robot.h"
#include "Product.h"
#include "Shelf.h"
#include "Dock.h"
#include "Truck.h"
#include "Order.h"
#include "DynamicOrderQueue.h"

class Computer {
  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;
  std::map<Product, int> inventory_;

  std::vector<Order> orders_;

  DynamicOrderQueue queue_;
  std::vector<Robot*> robots_;

  std::vector<Shelf> shelves_;
  std::vector<Dock> docks_;
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
        safe_printf("Layout loaded\n");
      }
  }

  void load_inventory() {
    std::ifstream fin("data/products.json");

    size_t nadded = 0;
    if (fin.is_open()) {
      JSON jinventory;
      fin >> jinventory;

      std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
      std::uniform_int_distribution<size_t> dist(1, 15);
      for (const auto& jitem : jinventory) {
        Product p(jitem["name"], (double)jitem["weight"]);
        int quantity = dist(rnd);
        auto it = inventory_.insert({ p, quantity });
      }
      safe_printf("Inventory loaded\n");

      load_shelves(inventory_);
      safe_printf("Shelves loaded\n");
    }
  }

  void load_shelves(std::map<Product, int> products) {
    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, shelves_.size() - 1);

    for (auto& pair : products) {
      int shelf = dist(rnd);
      safe_printf("\tPlacing %d %s on Shelf %d\n", pair.second, pair.first.name_.c_str(), shelf);     
      bool is_new = shelves_[shelf].add(pair.first, pair.second);
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
          std::pair<int, int> loc(c, r);
          Dock d(loc);
          docks_.push_back(d);
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
    safe_printf("Robots initialized\n");
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
    int next_id = orders_.size();
    o.id_ = next_id;
    safe_printf("Adding order %d. Order contains:\n", o.id_);
    for (const auto& p : o.products_) {
      safe_printf("\t%d %s\n", p.second, p.first.name_.c_str());
    }

    {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      std::vector<std::pair<int, int>> route;
      for (auto& pair : o.products()) {
        safe_printf("\tLooking for %d %s\n", pair.second, pair.first.name_.c_str());
        int amount = pair.second;

        if (inventory_[pair.first] - amount < 0) {
          safe_printf("NOT ENOUGH\n");
          return;
        }

        // remove from inventory
        inventory_[pair.first] -= amount;
        // remove from shelf
        for (Shelf& s : shelves_) {
          if (s.has_product(pair.first)) {
            int n = s.remove(pair.first, amount);
            amount -= n;
            std::pair<int, int> loc = s.get_location();
            safe_printf("\t%d taken off shelf at location {%d, %d}\n", n, loc.first, loc.second);
            loc.first += 1;
            route.push_back(loc);
          }

          if (amount == 0) break;
        }
      }

      if (route.size() > 0) o.set_route(route);
      o.set_status(PLACED);
      orders_.push_back(o);
    }
    queue_.add(o);
  }

  void arrive(Truck& t) {
    
  }

  void test_order_completion() {
    Product p("Chopsticks", 1.0);
    Order o(3, PLACED);
    o.add(p, 1);
    add_order(o);
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
    safe_printf("\n---\nWAREHOUSE MANAGER, ENTER A COMMAND\n0: Quit\n1: Spawn Robot\n2: View Inventory\n3: View Stock\n4: Show Orders\n5: View Shelf\n6: Test Orders\n7: Test Placed Order\n---\n\n");
  }

  int view_stock(std::string& name) {
    safe_printf("Looking for %s\n", name.c_str());
    Product p(name, 0);
    auto it = inventory_.find(p);
    if (it != inventory_.end()) {
      return it->second;
    }

    return -1;
  }

  void view_inventory() {
    for (const auto& pair : inventory_) {
      safe_printf("%s: %d\n", pair.first.name_.c_str(), pair.second);
    }
    safe_printf("\n");
  }

  void view_shelves() {
    for (int i = 0; i < shelves_.size(); i++) {
      safe_printf("Shelf %d contains:\n", i);
      for (auto& pair : shelves_[i].products()) {
        safe_printf("%d %s\n", pair.second, pair.first.name_.c_str());
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