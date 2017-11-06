#ifndef MIDTERM_EXCEPTION_H
#define MIDTERM_EXCEPTION_H

#include <exception>
#include <string>

/**
 * Simple throwable exception that can store
 *   a custom error message
 */
class TestException : public std::exception {
 private:
  std::string msg_;
 public:
  TestException(const std::string& msg) : msg_(msg) {}

/**
   * Custom error message
   * @return the explanatory string
   */
  const char* what() const noexcept {
    return msg_.c_str();
  }
};

#endif //MIDTERM_EXCEPTION_H
