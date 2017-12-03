#ifndef AMAZOOM_ORDER_H
#define AMAZOOM_ORDER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <json.hpp>
using JSON = nlohmann::json;

#include "Product.h"

#define NEW_ORDER 0
#define PLACED 1
#define READY 2
#define DELIVERED 3

class Order {
  std::vector<Product> products_;
  std::vector<std::pair<int, int>> route_;

 public:
  int id_;
  int status_;
  
  Order(int id, int status) : id_(id), status_(status), products_() { }

  static JSON toJSON(Order& o) {
    JSON j;
    j["id"] = o.id_;
    j["status"] = o.status_;
    j["products"] = Product::toJSON(o.products());
    return j;
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
    auto it = std::find(products_.begin(), products_.end(), p);

    if (it != products_.end()) products_.erase(it);
  }

  std::vector<Product>& products() {
    return products_;
  }

  friend bool operator<(const Order& a, const Order& b) {
    return a.id_ < b.id_;
  }

  friend bool operator==(const Product& a, const Product& b) {
    return a.id_ == b.id_;
  }

};

#endif