#include "word_count.h"

std::string trim(const std::string& s) {
  unsigned int begin = s.find_first_not_of(' ');
  unsigned int end = s.find_last_not_of(' ');
  return s.substr(begin, (end - begin) + 1);
}

int word_count(const std::string& line, int start_idx) {
  int length = line.length();
  if (length == 0) return 0;

  bool is_space = true;
  int nwords = 0;
  int i = start_idx;
  while (i < length) {
    if (is_space && line[i] != ' ') {
      is_space = false;
      nwords++;
    } else if (line[i] == ' ') {
      is_space = true;
    }

    i++;
  }

  return nwords;
}