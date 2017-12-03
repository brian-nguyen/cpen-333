#ifndef AMAZOOM_PRODUCT_H
#define AMAZOOM_PRODUCT_H

#include <iostream>
#include <string>
#include <vector>

class Product {
 public:
  std::string name_;
  double weight_;
  int quantity_;

  Product(std::string name, double weight, int quantity) {
    name_ = name;
    weight_ = weight;
    quantity_ = quantity;
  }

  static JSON toJSON(const Product& p) {
    JSON j;
    j["name"] = p.name_;
    j["weight"] = p.weight_;
    j["quantity"] = p.quantity_;
    return j;
  }

  static JSON toJSON(const std::vector<Product>& products) {
    JSON j;
    for (const auto& p : products) {
      j.push_back(toJSON(p));
    }
    return j;
  }
  
  friend bool operator<(const Product& a, const Product& b) {
    return a.name_ < b.name_;
  }

  friend bool operator==(const Product& a, const Product& b) {
    return a.name_ == b.name_;
  }
};

#endif