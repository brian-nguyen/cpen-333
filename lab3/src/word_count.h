#ifndef LAB3_WORD_COUNT_H
#define LAB3_WORD_COUNT_H

#include <string>

/**
 * Counts number of words, separated by spaces, in a line.
 * @param line string in which to count words
 * @param start_idx starting index to search for words
 * @return number of words in the line
 */
int word_count(const std::string& line, int start_idx);

#endif //LAB3_WORD_COUNT_H