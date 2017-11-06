#ifndef MIDTERM_Q1_SPINLOCK_H
#define MIDTERM_Q1_SPINLOCK_H

#include <atomic>
#include <thread>
#include <iostream>

class SpinLock {
  std::atomic_flag flag_;

 public:

  SpinLock() : flag_{ATOMIC_FLAG_INIT} {}

  /**
   * Acquires the lock, blocking if necessary
   */
  void lock() {
    //=======================================
    // TODO: Implement lock operation
    //=======================================
    while (flag_.test_and_set()) {}
  }

  /**
   * Tries to acquire the lock
   * @return true if lock is available and acquired successfully, false otherwise
   */
  bool try_lock() {
    //=======================================
    // TODO: Implement try_lock operation
    //=======================================
    
    // lock's previous value is true,
    // so failed to acquire lock
    if (flag_.test_and_set()) {
      return false;
    }

    // lock's previous value is false,
    // so successfully acquired lock
    return true;
  }

  /**
   * Releases or unlocks the lock, regardless of who may have locked it
   */
  void unlock() {
    //=======================================
    // TODO: Implement unlock operation
    //=======================================
    flag_.clear();
  }
};

#endif

