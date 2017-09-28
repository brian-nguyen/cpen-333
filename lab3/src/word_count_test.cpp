#include "word_count.h"
#include <exception>
#include <iostream>

// Exception class to throw for unit test failures
class UnitTestException : public std::exception {
  std::string line_;
  int start_idx_;
  int result_;
  int expected_;

 public:

  // constructor collecting all information
  UnitTestException(const std::string& line, int start_idx,
                    int result, int expected) : line_(line), start_idx_(start_idx),
                                                result_(result), expected_(expected) {}

  const char* what() {
    return "Unit test failed";
  }

  std::string info() {
    std::string out;
    out.append("line: ");
    out.append(line_);
    out.append(", start_idx: ");
    out.append(std::to_string(start_idx_));
    out.append(", result: ");
    out.append(std::to_string(result_));
    out.append(", expected: ");
    out.append(std::to_string(expected_));
    return out;
  }

};

/**
 * Tests word_count for the given line and starting index
 * @param line line in which to search for words
 * @param start_idx starting index in line to search for words
 * @param expected expected answer
 * @throws UnitTestException if the test fails
 */
void wc_tester(const std::string& line, int start_idx, int expected) {

  int result = word_count(line, start_idx);

  // if not what we expect, throw an error
  if (result != expected) {
    throw UnitTestException(line, start_idx, result, expected);
  }

}

int main() {
  try {
    // ideal case
    wc_tester("hello world", 0, 2);
    // empty
    wc_tester("", 0, 0);
    // one space
    wc_tester(" ", 0, 0);
    // extra whitespace between words
    wc_tester("hello  world", 0, 2);
    // only one word
    wc_tester("helloworld", 0, 1);
    // leading whitespace
    wc_tester(" hello world", 0, 2);
    // whitespace on both ends
    wc_tester(" hello world   ", 0, 2);
    // start on index of space
    wc_tester("hello world", 5, 1);
    // start on index of 2nd word
    wc_tester("hello world   ", 6, 1);    

    std::cout << "Passed all tests" << std::endl;  
  } catch(UnitTestException &ute) {
    std::cout << ute.info() << std::endl;
  }

}