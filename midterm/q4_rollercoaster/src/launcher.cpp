#include <cpen333/process/subprocess.h>
#include <thread>
#include <chrono>

// NOTE: I have a pwd issue. Changed paths to executables.
int main() {

  // launch a single roller coaster
  cpen333::process::subprocess rollercoaster("./q4_rollercoaster/bin/rollercoaster", true, true);

  // launch a bunch of persons
  const int npeople = 7;
  for (int i = 0; i < npeople; ++i) {
    cpen333::process::subprocess person("./q4_rollercoaster/bin/passenger", true, true);
  }

  // waits 10 seconds
  std::this_thread::sleep_for(std::chrono::seconds(5));
  for (int i = 0; i < npeople; ++i) {
    cpen333::process::subprocess person("./q4_rollercoaster/bin/passenger", true, true);
  }

  std::this_thread::sleep_for(std::chrono::seconds(20));  
  return 0;
}