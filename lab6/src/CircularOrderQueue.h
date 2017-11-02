#ifndef LAB6_CIRCULARORDERQUEUE_H
#define LAB6_CIRCULARORDERQUEUE_H

#include "OrderQueue.h"
#include <cpen333/thread/semaphore.h>
#include <mutex>

#define CIRCULAR_BUFF_SIZE 8

/**
 * Queue implementation using a circular buffer
 * (i.e. a fixed-size queue)
 */
class CircularOrderQueue : public virtual OrderQueue {
  Order buff_[CIRCULAR_BUFF_SIZE];
  cpen333::thread::semaphore producer_;
  cpen333::thread::semaphore consumer_;
  std::mutex pmutex_;
  std::mutex cmutex_;
  size_t pidx_;
  size_t cidx_;


 public:
  /**
   * Creates a queue with provided circular buffer size
   * @param buffsize size of circular buffer
   */
  CircularOrderQueue() :
      buff_(),
      producer_(CIRCULAR_BUFF_SIZE), consumer_(0),
      pmutex_(), cmutex_(), pidx_(0), cidx_(0){}

  void add(const Order& order) {
    producer_.wait();

    std::lock_guard<decltype(pmutex_)> lock(pmutex_);
    int pidx = pidx_;
    pidx_ = (pidx_ + 1) % CIRCULAR_BUFF_SIZE;
    buff_[pidx] = order;
    
    consumer_.notify();
  }

  Order get() {
    consumer_.wait();

    std::lock_guard<decltype(cmutex_)> lock(cmutex_);
    int cidx = cidx_;
    cidx_ = (cidx_ + 1) % CIRCULAR_BUFF_SIZE;
    Order out = buff_[cidx];

    producer_.notify();
    return out;
  }

};

#endif //LAB6_CIRCULARORDERQUEUE_H
