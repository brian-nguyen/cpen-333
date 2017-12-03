#ifdef AMAZOOM_PRODUCT
#define AMAZOOM_PRODUCT

#include <iostream>
#include <string>

class Product {
  int quantity_;
  double weight_;
  std::string name_;

 public:
  Product(int quantity, double weight, std::string name) {
    quantity_ = quantity;
    weight_ = weight;
    name_ = name
  }

  int get_quantity() {
    return quantity_;
  }

  void set_quantity(int n) {
    quantity_ = n
  }

  std::string get_name() {
    return name_;
  }
};

#endif