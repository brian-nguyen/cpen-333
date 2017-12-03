#ifndef AMAZOOM_ORDER_H
#define AMAZOOM_ORDER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "Product.h"

#define NEW_ORDER 0

class Order {
  int id_;
  int status_;
  std::vector<Product> products_;
  std::vector<std::pair<int, int>> route_;

 public:
  Order(int id, int status) : id_(id), status_(status), products_() { }

  int get_status() {
    return status_;
  }

  void set_status(int new_status) {
    status_ = new_status;
  }

  void set_route(std::vector<std::pair<int, int>> r) {
    route_ = r;
  }

  bool add(Product p) {
    for (Product& product : products_) {
      if (product == p) {
        product.quantity_ += p.quantity_;
        return false;
      }
    }

    products_.push_back(p);
    return true;
  }

  void remove(Product p) {
    products_.erase(std::find(products_.begin(), products_.end(), p));
  }

  const std::vector<Product>& products() const {
    return products_;
  }

};

#endif