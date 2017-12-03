#ifndef AMAZOOM_ROBOT_H
#define AMAZOOM_ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "common.h"
#include "safe_printf.h"

#include "Order.h"
#include "DynamicOrderQueue.h"

#define TAKEN '*'

class Robot : public cpen333::thread::thread_object {
  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;

  WarehouseInfo winfo_;
  int id_;
  int loc_[2];

  DynamicOrderQueue& queue_;

  void random_travel() {
    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rdist(0, winfo_.rows);
    std::uniform_int_distribution<int> cdist(0, winfo_.cols);

    while (1) {
      std::pair<int, int> goal(0, 0);
      do {
        goal.first = rdist(rnd);
        goal.second = cdist(rnd);
      } while (winfo_.warehouse[goal.first][goal.second] != EMPTY_CHAR);
      
      winfo_ = memory_->winfo;
      int i = go(goal);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
  
  public:
    Robot(DynamicOrderQueue& queue) :
      id_(0), memory_(SHARED_MEMORY_NAME), mutex_(SHARED_MUTEX_NAME), queue_(queue) {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      winfo_ = memory_->winfo;
      id_ = memory_->rinfo.nrobots;
      memory_->rinfo.nrobots++;

      loc_[COL_IDX] = memory_->rinfo.rloc[id_][COL_IDX];
      loc_[ROW_IDX] = memory_->rinfo.rloc[id_][ROW_IDX];
    }

    int get_id() {
      return id_;
    }

    int go(std::pair<int, int>& goal) {
      // get current location
      int c = loc_[COL_IDX];
      int r = loc_[ROW_IDX];

      // go back if on shelf or wall or dock or taken
      if (
        winfo_.warehouse[c][r] == WALL_CHAR ||
        winfo_.warehouse[c][r] == SHELF_CHAR ||
        winfo_.warehouse[c][r] == DOCK_CHAR ||
        winfo_.warehouse[c][r] == TAKEN
      ) return 0;

      // update UI
      memory_->rinfo.rloc[id_][COL_IDX] = c;
      memory_->rinfo.rloc[id_][ROW_IDX] = r;

      if (c == goal.first && r == goal.second) return 1;

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // mark current position
      winfo_.warehouse[c][r] = TAKEN;

      // go right
      if (c < winfo_.cols) {
        loc_[COL_IDX] = c + 1;
        loc_[ROW_IDX] = r;
        if (go(goal) == 1) return 1;
      }

      // go left
      if (c > 0) {
        loc_[COL_IDX] = c - 1;
        loc_[ROW_IDX] = r;
        if (go(goal) == 1) return 1;
      }

      // go down
      if (r < winfo_.rows) {
        loc_[COL_IDX] = c;
        loc_[ROW_IDX] = r + 1;
        if (go(goal) == 1) return 1;
      }

      // go up
      if (r > 0) {
        loc_[COL_IDX] = c;
        loc_[ROW_IDX] = r - 1;
        if (go(goal) == 1) return 1;
      }

      // backtrack
      winfo_.warehouse[c][r] = EMPTY_CHAR;

      // update UI
      memory_->rinfo.rloc[id_][COL_IDX] = c;
      memory_->rinfo.rloc[id_][ROW_IDX] = r;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));      
      return 0;
    }

    int restock() {
      return 0;
    }

    int main() {
      // randomly go to locations in warehouse
      // random_travel();

      // acquire first order
      Order o = queue_.get();
      safe_printf("Robot %d acquired Order %d\n", id_, o.id_);
      while (1) {
        for (auto& pair : o.route()) {
          go(pair);
        }

        // completed order
        o.set_status(READY);
        safe_printf("Robot %d completed Order %d on location {%d, %d}\n", id_, o.id_, o.route().back().first, o.route().back().second);

        // acqure next order
        o = queue_.get();
        safe_printf("Robot %d acquired Order %d\n", id_, o.id_);
      }

      return 1;
    }
};

#endif