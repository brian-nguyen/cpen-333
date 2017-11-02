#ifndef LAB6_SERVER_H
#define LAB6_SERVER_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Customer.h"
#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * Servers take completed orders from the chef and deliver
 * them to customers.
 */
class Server : public cpen333::thread::thread_object {
  OrderQueue& queue_;
  const std::vector<Customer*>& customers_;
  int id_;

 public:
  /**
   * Create a server
   * @param id server id
   * @param queue queue of completed orders to server.  It is assumed this vector of customers never changes.
   * @param customers list of customers for finding who to deliver order to
   */
  Server(int id, OrderQueue& queue,
         const std::vector<Customer*>& customers) :
      id_(id), queue_(queue), customers_(customers) {}

  /**
   * Unique server id
   * @return server id
   */
  int id() {
    return id_;
  }

  /**
   * Main work function
   * @return 0 when complete
   */
  int main() {

    safe_printf("Server %d started\n", id_);

    //==================================================
    // TODO: Detect when it's time to exit loop
    //==================================================
    Order order = queue_.get();
    while (true) {

      // serve order
      safe_printf("Server %d serving {%d,%d}\n", id_, order.customer_id, order.item_id);

      // Go find customer and serve
      for (auto& customer : customers_) {
        if (customer->id() == order.customer_id) {
          customer->serve(order);
          break;
        }
      }

      // next order
      order = queue_.get();
    }

    safe_printf("Server %d done.\n", id_);

    return 0;
  }
};

#endif //LAB6_SERVER_H
