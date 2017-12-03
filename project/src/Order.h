#ifndef AMAZOOM_ORDER_H
#define AMAZOOM_ORDER_H

#include <iostream>
#include <vector>

#define NEW_ORDER 0

class Order {
  int id_;
  int status_;

 public:
  Order(int id, int status) : id_(id), status_(status) { }

  int get_status() {
    return status_;
  }

  void set_status(int new_status) {
    status_ = new_status;
  }

  void set_route(std::vector<int> r) {
    std::cout << "Setting route" << std::endl;
  }

};

#endif