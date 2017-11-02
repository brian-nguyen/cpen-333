#ifndef LAB6_CHEF_H
#define LAB6_CHEF_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * The Chef grabs orders from a queue, cooks them,
 * then adds the cooked dishes to a new queue for
 * the servers to serve
 */
class Chef : public cpen333::thread::thread_object {
  OrderQueue& orders_;
  OrderQueue& serve_;
  int id_;
 public:
  /**
   * Create a new chef
   * @param id the chef's id
   * @param orders queue to read orders from
   * @param serve queue to add completed orders to
   */
  Chef(int id, OrderQueue& orders, OrderQueue& serve) :

      id_(id), orders_(orders), serve_(serve) {}

  int id() {
    return id_;
  }

  /**
   * Main execution function
   * @return 0 if completed
   */
  int main() {

    safe_printf("Chef %d started\n", id_);

    //==================================================
    // TODO: Detect when it's time to exit loop
    //==================================================
    Order order = orders_.get();
    while (true) {

      // process order
      safe_printf("Chef %d starting order {%d,%d}\n", id_, order.customer_id, order.item_id);
      std::this_thread::sleep_for(std::chrono::seconds(5));
      safe_printf("Chef %d completed order {%d,%d}\n", id_, order.customer_id, order.item_id);

      // add to those to serve
      serve_.add(order);

      // next order
      order = orders_.get();
    }
    safe_printf("Chef %d done\n", id_);

    return 0;
  }
};

#endif //LAB6_CHEF_H
