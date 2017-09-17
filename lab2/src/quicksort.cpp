#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// checks if sorted
bool isSorted(std::vector<int>& data) {
  for (int i = 0; i < data.size() - 1; i++) {
    if (data[i] > data[i + 1]) return false;
  }
  return true;
}

// utility to print all elements
void printArray(std::vector<int>& data) {
  for (int k = 0; k < data.size(); k++) {
    std::cout << data[k] << " ";
  }
  std::cout << std::endl;
}

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
// chooses the highest element as pivot
size_t partition2(std::vector<int>& data, int low, int high) {
  int i = low - 1;

  // go through array and swap smaller
  // element with one greater than the pivot
  for (int j = low; j < high - 1; j++) {
    if (data[j] < data[high]) {
      // move left pointer forward
      i++;
      std::swap(data[i], data[j]);
    }
  }

  // move pivot into position ahead 
  // of where left pointer stopped
  // this is the partitioning index
  if (data[i + 1] > data[high]) std::swap(data[i + 1], data[high]);
  return i + 1;
}

size_t partition(std::vector<int>& data, int low, int high) {
  int pivot = data[high];
  int i = low;
  int j = high;
  while (1) {
    // find two elements to be swapped
    // move both pointers in
    while (data[i] < pivot) i++;
    while (data[j] > pivot) j--;

    // if pointers have met
    if (i >= j) return j;

    std::swap(data[i], data[j]);
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
  if (low < high) {
    int p_idx = partition(data, low, high);

    // sort 1 partition in different thread
    std::thread* sortLeft = new std::thread(parallel_quicksort, std::ref(data), low, p_idx - 1);
    quicksort(data, p_idx + 1, high);
    sortLeft->join();
  }
}

int main() {

  // create two copies of random data
  const int VECTOR_SIZE = 1000000;
  std::vector<int> v1(VECTOR_SIZE, 0);
  // fill with random integers
  for (int i=0; i < VECTOR_SIZE; ++i) {
    v1[i] = rand();
  }
  std::vector<int> v2 = v1;  // copy all contents

  // sort v1 using sequential algorithm
  auto t1 = std::chrono::high_resolution_clock::now();
  quicksort(v1, 0, v1.size() - 1);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  long ms = duration_ms.count();
  std::cout << "Sequential duration: " << ms << " ms " << std::endl;

  // sort v2 using parallel algorithm
  t1 = std::chrono::high_resolution_clock::now();
  // parallel_quicksort(v2, 0, v2.size() - 1);
  t2 = std::chrono::high_resolution_clock::now();
  duration = t2-t1;
  duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  ms = duration_ms.count();
  std::cout << "Parallel duration: " << ms << " ms" << std::endl;

  // TEST
  if (isSorted(v1)) {
    std::cout << "SEQUENTIAL SORT PASSED\n" << std::endl;
  } else {
    std::cout << "SEQUENTIAL SORT FAILED\n" << std::endl;
  }

  if (isSorted(v2)) {
    std::cout << "PARALLEL SORT PASSED" << std::endl;
  } else {
    std::cout << "PARALLEL SORT FAILED" << std::endl;
  }

  return 0;
}