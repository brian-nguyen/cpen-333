#ifndef LAB6_DYNAMICORDERQUEUE_H
#define LAB6_DYNAMICORDERQUEUE_H

#include "OrderQueue.h"
#include <deque>
#include <condition_variable>
#include <mutex>

/**
 * Dynamically-sized Queue Implementation
 *
 * Does not block when adding items
 */
class DynamicOrderQueue : public virtual OrderQueue {
  std::deque<Order> buff_;
  std::mutex mutex_;
  std::condition_variable cv_;

 public:
  /**
   * Creates the dynamic queue
   */
  DynamicOrderQueue() :
      buff_(), mutex_(), cv_(){}

  void add(const Order& order) {
    {
      std::lock_guard(decltype(mutex_)) lock(mutex_);
      buff_.push_back(order);
    }
    cv_.notify_one();
  }

  Order get() {
    {
      std::lock_guard(decltype(mutex_)) lock(mutex_);
      cv.wait(lock, [&](){ return !buff_.empty() })      
      Order out = buff_.front();
      buff_.pop_front();
    }
    return out;
  }
};

#endif //LAB6_DYNAMICORDERQUEUE_H
