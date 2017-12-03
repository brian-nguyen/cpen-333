#ifndef AMAZOOM_PRODUCT_H
#define AMAZOOM_PRODUCT_H

#include <iostream>
#include <string>

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
  
  friend bool operator<(const Product& a, const Product& b) {
    return a.name_ < b.name_;
  }

  friend bool operator==(const Product& a, const Product& b) {
    return a.name_ == b.name_;
  }
};

#endif