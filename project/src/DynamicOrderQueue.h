#ifndef AMAZOOM_DYNAMIC_QUEUE_H
#define AMAZOOM_DYNAMIC_QUEUE_H

#include <deque>
#include <condition_variable>
#include <mutex>

#include "Order.h"

class DynamicOrderQueue {
  std::deque<Order> buff_;
  std::mutex mutex_;
  std::condition_variable cv_;

 public:
  DynamicOrderQueue() : buff_(), mutex_(), cv_() { }

  void add(const Order& order) {
    std::lock_guard<decltype(mutex_)> lock(mutex_);
    buff_.push_back(order);
    cv_.notify_one();
  }

  Order get() {
    std::unique_lock<decltype(mutex_)> lock(mutex_);
    cv_.wait(lock, [&](){ return !buff_.empty(); });
    Order out = buff_.front();
    buff_.pop_front();
    return out;
  }

};

#endif