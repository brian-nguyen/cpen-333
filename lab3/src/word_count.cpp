#include "word_count.h"

std::string trim(const std::string& s) {
  unsigned int begin = s.find_first_not_of(' ');
  unsigned int end = s.find_last_not_of(' ');
  return s.substr(begin, (end - begin) + 1);
}

int word_count(const std::string& line, int start_idx) {
  std::string trimmed = trim(line);
  int length = trimmed.length();
  int nwords = 0;
  
  int i = 0;
  while (i < length) {
    // if space encountered, run through
    // all spaces and increment nwords
    if (trimmed[i] == ' ') {
      while (trimmed[i] == ' ') i++;
      nwords++;
    }

    i++;

    // at end of line, increment nwords
    if (i == length && trimmed[i] != ' ') {
      nwords++;
    }
  }

  return nwords;
}