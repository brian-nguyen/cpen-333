#include "turtles.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/subprocess.h>
#include <thread>

int main() {

  cpen333::process::shared_object<TurtleMemory> memory(TURTLE_MEMORY_NAME);
  cpen333::process::mutex mutex(TURTLE_MUTEX_NAME);
  
  //========================================================
  // TODO: Implement Michelangelo
  //       - Initialize his memory
  //       - check-in
  //       - check if defeated
  //       - check on other turtle(s), restart if necessary
  //========================================================

  // re-initialize my memory
  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    memory->michelangelo.attacked = 0;
    memory->michelangelo.checkin = 0;
  }

  // take care of raphael
  int last_raphael = -1;
  int next_raphael = 0;
  int miss = 0;
  while (true) {
    {
      std::lock_guard<decltype(mutex)> lock(mutex);
      if (memory->michelangelo.attacked == 1) {
        std::cout << "michelangelo defeated" << std::endl;
        break;
      }
      memory->michelangelo.checkin++;
      next_raphael = memory->raphael.checkin;
    }

    // check on leonardo and restart if necessary
    if (last_raphael == next_raphael) {
      ++miss;
      if (miss == 2) {
        // restart raphael
        std::cout << "picking up raphael" << std::endl;
        miss = 0;
        cpen333::process::subprocess raph("./q3_turtles/bin/raphael", true, true);
      }
    }

    last_raphael = next_raphael;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}