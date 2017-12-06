#ifndef AMAZOOM_TRUCK_H
#define AMAZOOM_TRUCK_H

#include <iostream>
#include <map>

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>

#include "common.h"
#include "safe_printf.h"
#include "Order.h"
#include "Product.h"

#define DELIVERY_TRUCK 0
#define INVENTORY_TRUCK 1

class Truck : public cpen333::thread::thread_object {
 public:
  int type_;
  int max_weight_;
  cpen333::thread::semaphore& semaphore_;

  Truck(int max_weight, cpen333::thread::semaphore& semaphore)
    : max_weight_(max_weight), semaphore_(semaphore) { }
  
  virtual bool add(Order& o) = 0;
  virtual bool add(Product p, int quantity) = 0;
  
  virtual int main() = 0;
};

class DeliveryTruck : public Truck {
  std::vector<Order> orders_;

 public:
  DeliveryTruck(double max_weight, cpen333::thread::semaphore& semaphore)
    : Truck(max_weight, semaphore), orders_() {
      type_ = DELIVERY_TRUCK;
    }

  bool add(Order& o) {
    orders_.push_back(o);
    return true;
  }

  bool add(Product p, int quantity) {
    return false;
  }

  int main() {
    safe_printf("I am a delivery truck. Waiting for dock...\n");
    semaphore_.wait();

    safe_printf("Dock acquired\n");
    while (orders_.size() != max_weight_) { }

    safe_printf("Delivery truck full. Leaving now...\n");
    semaphore_.notify();
    return 0;
  }
};

class InventoryTruck : public Truck {
  std::map<Product, int> products_;

 public:
  InventoryTruck(double max_weight, cpen333::thread::semaphore& semaphore)
  : Truck(max_weight, semaphore), products_() {
    type_ = INVENTORY_TRUCK;
  }

  bool add(Order& o) {
    return false;
  }

  bool add(Product p, int quantity) {
    products_.insert({p, quantity });
    return true;
  }

  void remove(Product& p) {
    auto it = products_.find(p);
    products_.erase(it);
  }

  int main() {
    safe_printf("I am an inventory truck. Waiting for dock...\n");
    semaphore_.wait();

    safe_printf("Dock acquired\n");
    while (products_.size() > 0) { } 

    safe_printf("Inventory truck empty. Leaving now...\n");
    semaphore_.notify();
    return 0;
  }
};

#endif