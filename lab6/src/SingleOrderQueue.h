#ifndef LAB6_SINGLEORDERQUEUE_H
#define LAB6_SINGLEORDERQUEUE_H

#include "OrderQueue.h"
#include <cpen333/thread/semaphore.h>

/**
 * Basic thread-safe queue implementation with a single item slot.
 */
class SingleOrderQueue : public virtual OrderQueue {
  Order order_;
  cpen333::thread::semaphore producer_;
  cpen333::thread::semaphore consumer_;

 public:
  SingleOrderQueue() :
      order_(), producer_(1), consumer_(0) {}

  void add(const Order& order) {
    //=============================================
    // TODO: Safely add item to "queue"
    //    - wait for empty slot
    //    - fill slot
    //    - notify others of item availability
    //=============================================
    producer_.wait();
    order_ = order;
    consumer_.notify();
  }

  Order get() {
    //=============================================
    // TODO: Safely remove item from "queue"
    //    - wait for slot to be filled
    //    - remove item
    //    - notify others of empty slot
    //=============================================

    consumer_.wait();
    Order out = order_;
    producer_.notify();
    return out;
  }

};

#endif //LAB6_SINGLEORDERQUEUE_H
