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
    producer_.wait();
    order_ = order;
    consumer_.notify();
  }

  Order get() {
    consumer_.wait();
    Order out = order_;
    producer_.notify();
    return out;
  }

};

#endif //LAB6_SINGLEORDERQUEUE_H
