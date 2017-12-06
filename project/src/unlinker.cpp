#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>

#include "common.h"

int main() {
  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);
  cpen333::process::mutex mutex(SHARED_MUTEX_NAME);

  memory.unlink();
  mutex.unlink();
  return 0;
}