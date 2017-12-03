#ifdef AMAZOOM_TRUCK
#define AMAZOOM_TRUCK

#include <iostream>

class Truck {
 public:
  double max_weight_;
  int status_;
  int dock_;

  Truck(double max_weight, int status, int dock)
    : max_weight_(max_weight), status_(status), dock_(dock) { }
};

class DeliveryTruck : Truck {
 public:
  bool add() {
    return true;
  }
};

class InventoryTruck : Truck {
 public:
  void remove() {
    std::cout << "Removing..." << std::endl;
  }
};

#endif