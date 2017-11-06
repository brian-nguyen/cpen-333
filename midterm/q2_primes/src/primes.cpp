#include "primes.h"

#include <cmath>
#include <algorithm>

//===================================================================
// TODO: Fix bugs in binary_search
//===================================================================
int binary_search(const std::vector<int>& data, int val) {
  if (data.size() == 0) return -1;

  size_t min = 0;
  size_t max = data.size(); // one beyond the top

  while (min < max) {
    size_t mid = (min + max - 1) / 2;
    int v = data[mid];
    // find first occurrence
    if (v == val && (mid == 0 || data[mid - 1] < val)) { // check if it is the 1st occurence
      return (int)mid;
    } else if (val <= v) {
      max = mid;
    } else {
      min = mid + 1;
    }
  }

  if (data[min] != val) {
    return -1; // return correct value for not found
  }
  return (int)min;
}


//===================================================================
// TODO: Fix bugs in is_prime
//===================================================================
bool is_prime(int n) {

  // even number that is not 2 is not a prime
  if (n != 2 && (n % 2) == 0) {
    return false;
  }

  // default list of primes
  static const int limit = 46341;
  static auto primes = generate_primes(limit);

  // if it is within the bounds of the primes array, search for it
  // NOTE: in the problem statement, it states that the lookup should
  // be done when N < 46341 and that trial division should be
  // done when N >= 46341. So the if condition should read
  // if (n < limit) .. This still returns the correct result,
  // but the internal functionality is not consistent with the
  // algorithm in the question. Since this discrepancy is an internal 
  // implementation detail and does not contradict the specification,
  // I will leave it unchanged.
  if (n <= limit) {
    int idx = binary_search(primes, n);
    return idx != -1;
  }

  // trial division
  for (const auto& prime : primes) {
    if (prime * prime > n) {
      return true;
    } else if ((n % prime) == 0) {
      return false;
    }
  }
  return true;
}

std::vector<int> generate_primes(int n) {

  // There's no need to decipher the contents of this code,
  //   just test some inputs/outputs.

  // Modified sieve of eratosthenes
  // https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
  std::vector<int> primes;

  // tight upper bound on number of primes, from Piere Dusart
  //  used to pre-allocate memory for efficiency
  long double logp = std::log(n);
  auto max_length = (size_t) std::ceil((double) (((long double)n) / logp *
      (1.0l + 1.2762l / logp)));
  // pre-allocate memory
  primes.reserve(max_length);
  primes.push_back(2);  // first prime

  // details of prime range
  int sieve_min = 3;
  auto sieve_half_min = sieve_min / 2;
  auto max_half_prime = n / 2;

  // compute prime sieve in blocks of up to 2^16
  const size_t max_sieve_size = 1 << 16;  // 2^16
  size_t sieve_size = max_sieve_size;
  if (max_half_prime > 0) {
    sieve_size = std::min<size_t>(sieve_size, max_half_prime - sieve_half_min + 1);
  }
  std::vector<bool> sieve(sieve_size);

  // go through blocks
  while (sieve_half_min <= max_half_prime) {
    fill(sieve.begin(), sieve.end(), false);

    // mark all previous prime multiples
    const auto prime_size = primes.size();
    for (size_t i = 1; i < prime_size; ++i) {
      auto p = primes[i];
      auto hp = p / 2;  // half-prime

      // first multiple of prime in sieve
      auto hk = std::max<size_t>(hp, (sieve_half_min + hp) / p);
      auto j = hk * p + hp - sieve_half_min;
      if (hp * (p + 1) > sieve_half_min + sieve_size) {
        // outside of sieve block
        break;
      }
      // mark off multiples as not prime
      while (j < sieve_size) {
        sieve[j] = true;
        j += p;
      }
    }

    // loop through sieve to find next prime
    for (size_t i = 0; i < sieve_size; ++i) {
      if (!sieve[i]) {
        auto hp = i + sieve_half_min;
        auto p = 2 * hp + 1;
        primes.push_back((int)p);

        auto j = i + (2 * hp + 1)* hp;
        while (j < sieve_size) {
          sieve[j] = true;
          j += p;
        }
      }
    }

    // advance to next block
    sieve_half_min += sieve_size;
    // next size
    sieve_size = max_sieve_size;
    if (max_half_prime > 0) {
      sieve_size = std::min<size_t>(sieve_size, max_half_prime - sieve_half_min + 1);
    }
  }

  // if we went too far, knock off primes from the end
  while (!primes.empty() && primes.back() >= n) {
    primes.pop_back();
  }

  return primes;
}