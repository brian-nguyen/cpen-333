#ifndef AMAZOOM_SHELF_H
#define AMAZOOM_SHELF_H

#include <vector>
#include <algorithm>

#include "Product.h"

class Shelf {
  double max_weight_;
  std::pair<int, int> location_;
  std::vector<Product> products_;

 public:
  Shelf(double max_weight, std::pair<int, int> location) :
    max_weight_(max_weight), location_(location), products_() { }
  
  std::pair<int, int> get_location() {
    return location_;
  }

  bool add(Product p) {
    auto it = std::find(products_.begin(), products_.end(), p);
    if (it != products_.end()) {
      it->quantity_ += p.quantity_;
      return false;
    }

    products_.push_back(p);
    return true;
  }

  void remove(Product p) {
    auto it = std::find(products_.begin(), products_.end(), p);

    if (it != products_.end()) products_.erase(it);    
  }

  std::vector<Product>& products() {
    return products_;
  }

};

#endif