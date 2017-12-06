#ifndef AMAZOOM_PRODUCT_H
#define AMAZOOM_PRODUCT_H

#include <string>
#include <vector>

#include <json.hpp>
using JSON = nlohmann::json;

class Product {
 public:
  std::string name_;
  double weight_;

  Product(std::string name, double weight) {
    name_ = name;
    weight_ = weight;
  }

  friend bool operator<(const Product& a, const Product& b) {
    return a.name_ < b.name_;
  }

  friend bool operator==(const Product& a, const Product& b) {
    return a.name_ == b.name_;
  }
};

#endif