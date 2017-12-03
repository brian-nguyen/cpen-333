#ifndef AMAZOOM_COMPUTER
#define AMAZOOM_COMPUTER

#include <iostream>
#include <vector>
#include <string>
#include <map>

class Computer {
  std::map<std::string, int> inventory_;
  std::vector<int> orders_;

 public:
  Computer() : inventory_(), orders_() { }

  std::vector<int> get_orders(int status) {
    return orders_;
  }
};

#endif