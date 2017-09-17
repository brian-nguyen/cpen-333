#include <thread>
#include <iostream>
#include <random>
#include <chrono>

double estimate_pi(int nsamples) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  // generate n points and test if
  // they are in the circle
  double numHits = 0.0;
  for (int i = 0; i < nsamples; i++) {
    double x = dist(rnd);
    double y = dist(rnd);
    // a point is in the circle
    // when x^2 + y^2 <= r^2
    if (std::pow(x, 2) + std::pow(y, 2) <= 1) numHits++;
  }

  double pi = numHits / nsamples;
  return pi * 4;
}

// generates a random sample and sets hits[idx]
// to true if within the unit circle
void pi_sampler(std::vector<bool>& hits, int idx) {
  // single instance of random engine and distribution
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  double x = dist(rnd);
  double y = dist(rnd);
  if (std::pow(x, 2) + std::pow(y, 2) <= 1) hits[idx] = true;
}

// naively uses multithreading to try to speed up computations
double estimate_pi_multithread_naive(int nsamples) {
  // stores whether each sample falls within circle
  std::vector<bool> hits(nsamples, false);

  // create and store all threads
  std::vector<std::thread> threads;
  for (int i = 0; i < nsamples; ++i) {
    threads.push_back(std::thread(pi_sampler, std::ref(hits), i));
  }

  // wait for all threads to complete
  for (int i = 0; i < nsamples; ++i) {
    threads[i].join();
  }

  // estimate our value of PI
  double pi = 0;
  for (int i = 0; i < nsamples; ++i) {
    if (hits[i]) {
      pi = pi + 1;
    }
  }
  pi = pi / nsamples * 4;

  return pi;
}

// count number of hits using nsamples, populates hits[idx]
void pi_hits(std::vector<int>& hits, int idx, int nsamples) {
  // single instance of random engine and distribution
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  for (int i = 0; i < nsamples; i++) {
    double x = dist(rnd);
    double y = dist(rnd);
    if (std::pow(x, 2) + std::pow(y, 2) <= 1) hits[idx]++;
  }
}

// divides work among threads intelligently
double estimate_pi_multithread(int nsamples) {
  // create nthread hit counters
  int nthreads = std::thread::hardware_concurrency();
  std::vector<int> hits(nthreads, 0);

  // create and store threads
  std::vector<std::thread> threads;
  int msamples = 0; // samples accounted for
  for (int i = 0; i < nthreads - 1; ++i) {
    threads.push_back(std::thread(pi_hits, std::ref(hits), i, nsamples / nthreads));
    msamples += nsamples / nthreads;
  }
  // remaining samples
  threads.push_back(std::thread(pi_hits, std::ref(hits), nthreads - 1, nsamples - msamples));

  // wait for threads to finish
  for (int i = 0; i < nthreads; ++i) {
    threads[i].join();
  }

  // estimate pi
  double pi = 0;
  for (int i = 0; i < nthreads; ++i) {
    pi += hits[i];
  }
  pi = pi / nsamples * 4;

  return pi;
}

long getDuration(std::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::nanoseconds> t1) {
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  long ms = duration_ms.count();
  return ms;
}

int main() {
  // pi = 3.1415926536

  // 1000 samples gets pi accurate to 1 decimal place
  // needs about 10 million samples to get 3 decimal place accuracy
  auto t1 = std::chrono::high_resolution_clock::now();  
  double pi = estimate_pi(10000000);
  long ms = getDuration(t1);
  std::cout << "My estimate of PI is: " << pi << std::endl;
  std::cout << "Duration: " << ms << " ms" << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  pi = estimate_pi_multithread_naive(1000);
  ms = getDuration(t1);
  std::cout << "\nNaive threaded estimate of PI is: " << pi << std::endl;
  std::cout << "Duration: " << ms << " ms" << std::endl;
  
  t1 = std::chrono::high_resolution_clock::now();
  pi = estimate_pi_multithread(10000);
  ms = getDuration(t1);
  std::cout << "\nThreaded estimate of PI is: " << pi << std::endl;
  std::cout << "Duration: " << ms << " ms" << std::endl;
  
  return 0;
}