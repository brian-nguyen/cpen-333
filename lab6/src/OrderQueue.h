#ifndef LAB6_QUEUE_H
#define LAB6_QUEUE_H

#include "Order.h"

/**
 * Thread-safe queue of items
 */
class OrderQueue {
 public:
  /**
   * Adds an order item to the queue
   * @param order item to add
   */
  virtual void add(const Order& order) = 0;

  /**
   * Retrieve the next order item in the queue
   * @return next available item
   */
  virtual Order get() = 0;

};

#endif //LAB6_QUEUE_H
