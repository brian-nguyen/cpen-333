#ifndef AMAZOOM_SHELF_H
#define AMAZOOM_SHELF_H

#include <map>
#include <vector>
#include <algorithm>

#include "Product.h"

class Shelf {
  double max_weight_;
  std::pair<int, int> location_;
  std::map<Product, int> products_;

 public:
  Shelf(double max_weight, std::pair<int, int> location) :
    max_weight_(max_weight), location_(location), products_() { }
  
  std::pair<int, int> get_location() {
    return location_;
  }

  bool add(Product p, int quantity) {
    auto it = products_.find(p);
    if (it != products_.end()) {
      products_[p] += quantity;
    }

    products_.insert({p, quantity});
    return true;
  }

  int remove(Product p, int quantity) {
    auto it = products_.find(p);
    if (it != products_.end()) {
      int n = it->second - quantity;
      
      if (n <= 0) {
        products_.erase(it);
        return n < 0 ? it->second : quantity;
      } else {
        products_[p] -= quantity;
        return quantity;
      }
    }

    return 0;
  }

  bool has_product(Product p) {
    auto it = products_.find(p);
    if (it != products_.end()) {
      return true;
    }

    return false;
  }

  std::map<Product, int>& products() {
    return products_;
  }

};

#endif