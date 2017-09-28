#include "word_count.h"

// implementation details
int word_count(const std::string& line, int start_idx) {

  // YOUR IMPLEMENTATION HERE

  int nwords = 0;
  for (int i = start_idx; i < line.length(); i++) {
    if (line[i] == ' ') {
      nwords++;
    }
  }

  return nwords;
}