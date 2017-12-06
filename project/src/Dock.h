#ifndef AMAZOOM_DOCK_H
#define AMAZOOM_DOCK_H

#include <iostream>
#include <map>

#include "Truck.h"

class Dock {
  std::pair<int, int> location_;
 public:
  bool is_free;
  Dock(std::pair<int, int> loc) : location_(loc), is_free(false) { }
};

#endif