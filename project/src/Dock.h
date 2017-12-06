#ifndef AMAZOOM_DOCK_H
#define AMAZOOM_DOCK_H

#include <iostream>
#include <map>

class Dock {
  std::pair<int, int> location_;
 public:
  Dock(std::pair<int, int> loc) : location_(loc) { }
};

#endif