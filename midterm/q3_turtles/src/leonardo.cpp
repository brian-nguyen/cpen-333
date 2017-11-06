#include "turtles.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/subprocess.h>
#include <thread>

int main() {

  cpen333::process::shared_object<TurtleMemory> memory(TURTLE_MEMORY_NAME);
  cpen333::process::mutex mutex(TURTLE_MUTEX_NAME);
  
  //========================================================
  // TODO: Implement Leonardo
  //       - Initialize his memory
  //       - check-in
  //       - check if defeated
  //       - check on other turtle(s), restart if necessary
  //========================================================

  // re-initialize my memory
  // protect access here
  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    memory->leonardo.attacked = 0;
    memory->leonardo.checkin = 0;
  }

  // take care of donatello
  int last_donatello = -1;
  int next_donatello = 0;
  int miss = 0;
  while (true) {
    // protect access here
    {
      std::lock_guard<decltype(mutex)> lock(mutex);
      // if attacked by Shredder, exit loop
      if (memory->leonardo.attacked == 1) {
        std::cout << "leonardo defeated" << std::endl;
        break;
      }
      memory->leonardo.checkin++;
      next_donatello = memory->donatello.checkin;
    }

    // check on donatello and restart if necessary
    if (last_donatello == next_donatello) {
      ++miss;

      // NOTE: It is possible to JUST miss a check in, so when 
      // that happens, leonardo may falsely pick up donatello
      // which results in an extra process running. To
      // prevent this, leonardo will pick up donatello
      // after 2 missed check ins. My assumption is that
      // there is no requirement to how fast a turtle must
      // be picked up after its down (I don't see any requirements)
      if (miss == 2) {
        // restart donny
        miss = 0;
        std::cout << "picking up donatello" << std::endl;
        // NOTE: due to my configuration, I have a pwd issue
        // I have modified the path to the executable
        cpen333::process::subprocess donny("./q3_turtles/bin/donatello", true, true);
        // cpen333::process::subprocess donny("./donatello", true, true);
      }
    }

    last_donatello = next_donatello;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}