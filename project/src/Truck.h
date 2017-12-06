#ifndef AMAZOOM_TRUCK_H
#define AMAZOOM_TRUCK_H

#include <iostream>
#include <vector>

#include "Order.h"
#include "Product.h"

class Truck {
 public:
  double max_weight_;
  int status_;
  int dock_;

  Truck(double max_weight, int status, int dock)
    : max_weight_(max_weight), status_(status), dock_(dock) { }
};

class DeliveryTruck : public Truck {
  std::vector<Order> orders_;

 public:
  DeliveryTruck(double max_weight, int status, int dock)
    : Truck(max_weight, status, dock), orders_() { }

  bool add() {
    return true;
  }
};

class InventoryTruck : public Truck {
  std::vector<Product> products_;

 public:
  InventoryTruck(double max_weight, int status, int dock)
  : Truck(max_weight, status, dock), products_() { }

  void remove() {
    std::cout << "Removing..." << std::endl;
  }
};

#endif