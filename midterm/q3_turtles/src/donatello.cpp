#include "turtles.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/subprocess.h>
#include <thread>

int main() {

  cpen333::process::shared_object<TurtleMemory> memory(TURTLE_MEMORY_NAME);
  cpen333::process::mutex mutex(TURTLE_MUTEX_NAME);
  
  //========================================================
  // TODO: Implement Donatello
  //       - Initialize his memory
  //       - check-in
  //       - check if defeated
  //       - check on other turtle(s), restart if necessary
  //========================================================

  // re-initialize my memory
  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    memory->donatello.attacked = 0;
    memory->donatello.checkin = 0;
  }

  // take care of michelangelo
  int last_michelangelo = -1;
  int next_michelangelo = 0;
  int miss = 0;
  while (true) {
    {
      std::lock_guard<decltype(mutex)> lock(mutex);
      if (memory->donatello.attacked == 1) {
        std::cout << "donatello defeated" << std::endl;
        break;
      }
      memory->donatello.checkin++;
      next_michelangelo = memory->michelangelo.checkin;
    }

    // check on michelangelo and restart if necessary
    if (last_michelangelo == next_michelangelo) {
      ++miss;
      if (miss == 2) {
        // restart mikey
        std::cout << "picking up michelangelo" << std::endl;
        miss = 0;
        cpen333::process::subprocess mikey("./q3_turtles/bin/michelangelo", true, true);
      }
    }

    last_michelangelo = next_michelangelo;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}