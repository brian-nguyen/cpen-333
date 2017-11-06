#include "common.h"

#include <cpen333/process/semaphore.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/condition_variable.h>

#include <thread>
#include <chrono>

int main() {

  //==========================================================
  // TODO: Implement roller coaster
  //   - Initialize anything that needs to be initialized
  //   - Loop
  //      - Notify passengers it is safe to board
  //      - Wait until full OR
  //          there is at least one passenger boarded AND
  //          no more passengers are in line
  //      - Go for ride
  //      - Notify passengers it is safe to exit
  //      - Wait until all passengers have exited
  //==========================================================

  // initialize
  cpen333::process::shared_object<RideData> ride(RIDE_SHARED_MEM);
  cpen333::process::condition_variable cv(LINE_CV);
  cpen333::process::mutex mutex(LINE_MUTEX);

  cpen333::process::semaphore board(BOARD_SEMAPHORE, 0);
  // cpen333::process::semaphore full(FULL_SEMAPHORE, 0);
  cpen333::process::semaphore exiting(EXIT_SEMAPHORE, 0);
  cpen333::process::semaphore empty(EMPTY_SEMAPHORE, 0);

  // keep running roller coaster forever
  while (true) {

    // Condition 1: Rollercoaster notifies BOARD_SEMAPHORE 4 times (4 seats available)
    std::cout << "Roller coaster loading passengers..." << std::endl;
    for (int i = 0; i < ROLLERCOASTER_CAPACITY; i++) {
      board.notify();
    }

    int npassengers = 0;
    // Attempt 1: Only moving at full capacity. This method doesn't let the 2 last passengers ride
    // because it is not at full capacity :( those poor souls. See attempt #2 where I help them out.
    // Condition 2: Wait until rollercoaster at capacity
    // for (int i = 0; i < ROLLERCOASTER_CAPACITY; i++) {
    //   full.wait();
    //   npassengers++;
    // }
    
    // Attempt 2: Using condition variable.
    // Condition 2: Use CV to block until either at capacity OR (passengers > 0 AND num_waiting == 0)
    // NOTE: Initially, this ride would take off instantly as the 1st passenger boards because 
    // the other passenger subprocesses had not started yet. I introduced this delay as the 
    // minimum amount of time a rollercoaster stays at the station
    // until it checks its condition to start the ride :)
    std::this_thread::sleep_for(std::chrono::seconds(1));    
    {
      std::unique_lock<decltype(mutex)> lock(mutex);
      cv.wait(lock, [&ride](){ return (ride->num_passengers == ROLLERCOASTER_CAPACITY) || (ride->num_passengers > 0 && ride->num_waiting == 0); });
      npassengers = ride->num_passengers;
    }

    // If running at less than full capacity, prevent passengers in line from boarding
    for (int i = 0; i < ROLLERCOASTER_CAPACITY - npassengers; i++) {
      board.wait();
    }

    // GO!
    std::cout << "Roller coaster running with " << npassengers << " passengers" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Roller coaster returned, letting passengers off" << std::endl;

    // Condition 3: Rollercoaster notifies EXIT_SEMAPHORE n times when ride is over
    for (int i = 0; i < npassengers; i++) {
      exiting.notify();
    }

    std::cout << "Roller coaster waiting for passengers to exit" << std::endl;

    // Condition 4: Rollercoaster waits until all passengers have exited
    for (int i = 0; i < npassengers; i++) {
      empty.wait();
    }

    // Clear shared memory of passengers
    {
      std::lock_guard<decltype(mutex)> lock(mutex);
      ride->num_passengers = 0;
    }

  }

  return 0;
}