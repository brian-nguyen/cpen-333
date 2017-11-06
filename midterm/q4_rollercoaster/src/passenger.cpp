#include "common.h"

#include "common.h"

#include <cpen333/process/semaphore.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/condition_variable.h>
#include <iostream>

int main() {
  //==========================================================
  // TODO: Implement passenger
  //   - Wait until roller coaster is ready and boarding
  //   - Notify roller coaster when boarded
  //   - Wait until ride is over and safe to leave
  //   - Notify roller coaster when leaving
  //==========================================================
  cpen333::process::shared_object<RideData> ride(RIDE_SHARED_MEM);
  cpen333::process::condition_variable cv(LINE_CV);  
  cpen333::process::mutex mutex(LINE_MUTEX);

  cpen333::process::semaphore board(BOARD_SEMAPHORE, 0);
  // cpen333::process::semaphore full(FULL_SEMAPHORE, 0);
  cpen333::process::semaphore exiting(EXIT_SEMAPHORE, 0);
  cpen333::process::semaphore empty(EMPTY_SEMAPHORE, 0);

  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    ride->num_waiting++;
  }
  std::cout << "Passenger waiting in line" << std::endl;

  // Condition 1: Wait until rollercoaster is available to board
  // The board semaphore will be notified by the rollercoaster
  // when it is ready for passengers
  board.wait();
  
  // Attempt 1: Using semaphore
  // Condition 2: Notify rollercoaster that passenger has boarded
  // full.notify();

  // Attempt 2: Using condition variable
  // Condition 2: Update shared memory so that rollercoaster can make a decision
  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    ride->num_waiting--;
    ride->num_passengers++;
  }
  cv.notify_all();
  std::cout << "Passenger boarded" << std::endl;
  
  // Condition 3: Wait until ride is over. Rollercoaster will notify
  // the exit semaphore when it is ready to let passengers off
  exiting.wait();
  
  // Condition 4: Notify rollercoaster that passenger has left
  empty.notify();
  std::cout << "Passenger leaving" << std::endl;

  return 0;
}