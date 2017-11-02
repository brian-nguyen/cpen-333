#ifndef LAB6_CUSTOMER_H
#define LAB6_CUSTOMER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>

#include "safe_printf.h"

/**
 * Customers place orders into a queue, wait for them to be
 * served, eat, then leave
 */
class Customer : public cpen333::thread::thread_object {
  OrderQueue& queue_;
  Menu& menu_;
  int id_;

 public:
  /**
   * Creates a customer
   * @param id customer id
   * @param menu menu for customer to order food from
   * @param queue queue to place order into
   */
  Customer(int id, Menu& menu, OrderQueue& queue) :
      id_(id), menu_(menu), queue_(queue) {}

  /**
   * Unique customer id
   * @return customer id
   */
  int id() {
    return id_;
  }

  /**
   * Server customer an order
   * @param order order that is complete
   */
  void serve(const Order& order) {

    //==================================================
    // TODO: Notify main method that order is ready
    //==================================================

  }

  /**
   * Main customer function
   * @return 0 when complete
   */
  int main() {

    safe_printf("Customer %d arrived\n", id_);

    // randomly order a bunch of food
    double cost = 0;
    int items = 0;

    srand((int)std::chrono::system_clock::now().time_since_epoch().count());

    // appetizer
    size_t s = menu_.appetizers().size();
    if (s > 0) {
      MenuItem appy = menu_.appetizers()[rand() % s];
      cost += appy.price;
      ++items;

      safe_printf("Customer %d ordering the %s (%d)\n", id_, appy.item.c_str(), appy.id);
      queue_.add({id_, appy.id});
    }

    // main course
    s = menu_.mains().size();
    if (s > 0) {
      MenuItem meal = menu_.mains()[rand() % s];
      cost += meal.price;
      ++items;

      safe_printf("Customer %d ordering the %s (%d)\n", id_, meal.item.c_str(), meal.id);
      queue_.add({id_, meal.id});
    }

    //==================================================
    // TODO: wait for meals to be served
    //==================================================

    // stay for some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    safe_printf("Customer %d paying $%.2f and leaving.\n", id_, cost);

    return 0;
  }
};

#endif //LAB6_CUSTOMER_H
