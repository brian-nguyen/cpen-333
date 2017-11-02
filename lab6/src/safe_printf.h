#ifndef LAB6_SAFEPRINT_H
#define LAB6_SAFEPRINT_H

#include <cstdio>
#include <mutex>
#include <cstdarg>

/**
 * Thread-safe printing to console (prevent garbled messages) using printf format
 * @param format printf format
 * @param ... list of additional arguments
 */
inline void safe_printf(char const * const format, ...) {
  std::va_list args;
  va_start(args, format);
  static std::mutex mutex;
  {
    std::lock_guard<decltype(mutex)> lock(mutex);
    std::vprintf(format, args);
  }
  va_end(args);
}

#endif //LAB6_SAFEPRINT_H
