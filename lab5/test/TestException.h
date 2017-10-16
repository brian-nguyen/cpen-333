#ifndef LAB5_TESTEXCEPTION_H
#define LAB5_TESTEXCEPTION_H

#include <string>
#include <exception>

/**
 * Custom exception class that allows us to specify an error message
 */
class TestException : public std::exception {
  const std::string msg_;

 public:
  TestException(const std::string& msg) : msg_(msg) {}

  const char* what() {
    return msg_.c_str();
  }
};

#endif //LAB5_TESTEXCEPTION_H
