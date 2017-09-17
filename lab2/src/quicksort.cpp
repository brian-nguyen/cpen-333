#include <iostream>
#include <thread>
#include <vector>

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
// chooses the highest element as pivot
size_t partition(std::vector<int>& data, int low, int high) {
  int pivot = data[high];
  int i = low;
  int j = high;
  
  while (i < j) {

    }
  }
}

void quicksort(std::vector<int>& data, int low, int high) {
  if (low < high) {
    int p_idx = partition(data, low, high);
    quicksort(data, low, p_idx - 1);
    quicksort(data, p_idx + 1, high);
  }
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
  // your code here
}

int main() {

  // create two copies of random data
  const int VECTOR_SIZE = 1000000;
  std::vector<int> v1(VECTOR_SIZE, 0);
  // fill with random integers
  for (int i=0; i<VECTOR_SIZE; ++i) {
    v1[i] = rand();
  }
  std::vector<int> v2 = v1;  // copy all contents

  // sort v1 using sequential algorithm
  quicksort(v1, 0, v1.size() - 1);

  // sort v2 using parallel algorithm
  parallel_quicksort(v2, 0, v2.size() - 1);

  return 0;
}