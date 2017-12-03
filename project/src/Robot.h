#ifndef AMAZOOM_ROBOT_H
#define AMAZOOM_ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "safe_printf.h"

#define WALL_CHAR 'X'
#define DOCK_CHAR 'D'
#define EMPTY_CHAR ' '
#define SHELF_CHAR 'S'

class Robot : public cpen333::thread::thread_object {
  int id_;

  public:
    Robot(int id) : id_(id) { }

    int get_id() {
      return id_;
    }

    int go() {
      return 1;
    }

    int restock() {
      return 0;
    }

    int main() {
      safe_printf("Robot %d starting...\n", id_);
      return 1;
    }
};

#endif