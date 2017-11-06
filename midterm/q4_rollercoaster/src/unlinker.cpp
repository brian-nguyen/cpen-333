#include "common.h"

#include "common.h"

#include <cpen333/process/semaphore.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/condition_variable.h>
#include <iostream>

/**
 * NOTE: I created this program to unlink all of the shared
 * objects I was creating since Mac has kernel persistence.
 * Not unlinking between running launcher.exe without reboots
 * caused very strange bugs such as getting stale semaphores
 * states from previous incarnations of launcher.exe
 */
int main() {
  cpen333::process::shared_object<RideData> ride(RIDE_SHARED_MEM);
  cpen333::process::condition_variable cv(LINE_CV);  
  cpen333::process::mutex mutex(LINE_MUTEX);

  cpen333::process::semaphore board(BOARD_SEMAPHORE, 0);
  cpen333::process::semaphore full(FULL_SEMAPHORE, 0);
  cpen333::process::semaphore exiting(EXIT_SEMAPHORE, 0);
  cpen333::process::semaphore empty(EMPTY_SEMAPHORE, 0);

  ride.unlink();
  cv.unlink();
  mutex.unlink();
  
  board.unlink();
  full.unlink();
  exiting.unlink();
  empty.unlink();

  std::cout << "Unlinked\n";
}