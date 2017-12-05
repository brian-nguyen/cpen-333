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

  static JSON toJSON(const Product& p, int quantity) {
    JSON j;
    j["name"] = p.name_;
    j["weight"] = p.weight_;
    j["quantity"] = quantity;
    return j;
  }

  static JSON toJSON(std::map<Product, int>& products) {
    JSON j;
    for (const auto& p : products) {
      j.push_back(toJSON(p.first, p.second));
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