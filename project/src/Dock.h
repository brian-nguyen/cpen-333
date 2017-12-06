#ifndef AMAZOOM_DOCK_H
#define AMAZOOM_DOCK_H

#include <iostream>
#include <map>

#include "Truck.h"

class Dock {
  std::pair<int, int> location_;
  Truck& truck_;
 public:
  Dock(std::pair<int, int> loc, Truck truck) : location_(loc), truck_(truck) { }
};

#endif