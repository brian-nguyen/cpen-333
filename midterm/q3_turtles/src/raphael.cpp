#include "turtles.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/subprocess.h>
#include <thread>

int main() {

  cpen333::process::shared_object<TurtleMemory> memory(TURTLE_MEMORY_NAME);
  cpen333::process::mutex mutex(TURTLE_MUTEX_NAME);
  
  //========================================================
  // TODO: Implement Raphael
  //       - Initialize his memory
  //       - check-in
  //       - check if defeated
  //       - check on other turtle(s), restart if necessary
  //========================================================

  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    memory->raphael.attacked = 0;
    memory->raphael.checkin = 0;
  }

  // take care of leonardo
  int last_leonardo = -1;
  int next_leonardo = 0;
  int miss = 0;
  while (true) {
    {
      std::lock_guard<decltype(mutex)> lock(mutex);
      if (memory->raphael.attacked == 1) {
        std::cout << "raphael defeated" << std::endl;
        break;
      }
      memory->raphael.checkin++;
      next_leonardo = memory->leonardo.checkin;      
    }

    if (last_leonardo == next_leonardo) {
      ++miss;

      if (miss == 2) {
        miss = 0;
        std::cout << "picking up leonardo" << std::endl;
        cpen333::process::subprocess leo("./q3_turtles/bin/leonardo", true, true);
      }
    }

    last_leonardo = next_leonardo;
    std::this_thread::sleep_for(std::chrono::seconds(5));    
  }

  return 0;
}