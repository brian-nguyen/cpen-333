#include <map>
#include <string>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

#include "word_count.h"

/**
 * Checks if the line specifies a character's dialogue, returning
 * the index of the start of the dialogue.  If the
 * line specifies a new character is speaking, then extracts the
 * character's name.
 *
 * Assumptions: (doesn't have to be perfect)
 *     Line that starts with exactly two spaces has
 *       CHARACTER. <dialogue>
 *     Line that starts with exactly four spaces
 *       continues the dialogue of previous character
 *
 * @param line line to check
 * @param character extracted character name if new character,
 *        otherwise leaves character unmodified
 * @return index of start of dialogue if a dialogue line,
 *      -1 if not a dialogue line
 */
int is_dialogue_line(const std::string& line, std::string& character) {

  // new character
  if (line.length() >= 3 && line[0] == ' '
      && line[1] == ' ' && line[2] != ' ') {
    // extract character name

    int start_idx = 2;
    int end_idx = 3;
    while (end_idx <= line.length() && line[end_idx-1] != '.') {
      ++end_idx;
    }

    // no name found
    if (end_idx >= line.length()) {
      return 0;
    }

    // extract character's name
    character = line.substr(start_idx, end_idx-start_idx-1);
    return end_idx;
  }

  // previous character
  if (line.length() >= 5 && line[0] == ' '
      && line[1] == ' ' && line[2] == ' '
      && line[3] == ' ' && line[4] != ' ') {
    // continuation
    return 4;
  }

  return 0;
}

/**
 * Reads a file to count the number of words each actor speaks.
 *
 * @param filename file to open
 * @param mutex mutex for protected access to the shared wcounts map
 * @param wcounts a shared map from character -> word count
 */
void count_character_words(const std::string& filename,
                           std::mutex& mutex,
                           std::map<std::string,int>& wcounts) {

  //===============================================
  //  IMPLEMENT THREAD SAFETY IN THIS METHOD
  //===============================================

  std::string line;  // for storing each line read from the file
  std::ifstream file (filename);

  // read contents of file if open
  if (file.is_open()) {

    std::string character = "";  // empty character to start

    // line by line
    while ( std::getline (file,line) ) {

      int idx = is_dialogue_line(line, character);
      if (idx > 0 && !character.empty()) {

        int nwords = word_count(line, idx);

        // add character if doesn't exist, otherwise increment count

        //=================================================
        // YOUR JOB TO ADD WORD COUNT INFORMATION TO MAP
        //=================================================

      } else {
        character = "";  // reset character
      }

    }
    file.close();  // close file
  }

}

/**
 * Comparator, orders by number decreasing, then by name
 * @param p1 first pair
 * @param p2 second pair
 * @return true if p1 should come before p2, false otherwise
 */
bool wc_greater_than(std::pair<std::string,int>& p1, std::pair<std::string,int>& p2) {

  //===============================================
  // YOUR IMPLEMENTATION HERE TO ORDER p1 AND p2
  //===============================================

  return false;
};

/**
 * Sorts characters in descending order by word count
 *
 * @param wcounts a map of character -> word count
 * @return sorted vector of {character, word count} pairs
 */
std::vector<std::pair<std::string,int>> sort_characters_by_wordcount(
    const std::map<std::string,int>& wcounts) {

  std::vector<std::pair<std::string,int>> out;
  out.reserve(wcounts.size());   // reserve memory for efficiency

  // sort characters by words descending
  for (const auto& pair : wcounts) {
    out.push_back(pair);
  }
  std::sort(out.begin(), out.end(), wc_greater_than);

  return out;
}

int main() {

  // map and mutex for thread safety
  std::mutex mutex;
  std::map<std::string,int> wcounts;

  std::vector<std::string> filenames = {
      "data/shakespeare_antony_cleopatra.txt",
      "data/shakespeare_hamlet.txt",
      "data/shakespeare_julius_caesar.txt",
      "data/shakespeare_king_lear.txt",
      "data/shakespeare_macbeth.txt",
      "data/shakespeare_merchant_of_venice.txt",
      "data/shakespeare_midsummer_nights_dream.txt",
      "data/shakespeare_much_ado.txt",
      "data/shakespeare_othello.txt",
      "data/shakespeare_romeo_and_juliet.txt",
  };

  //=============================================================
  // YOUR IMPLEMENTATION HERE TO COUNT WORDS IN MULTIPLE THREADS
  //=============================================================

  auto sorted_wcounts = sort_characters_by_wordcount(wcounts);

  // results
  for (const auto& entry : sorted_wcounts) {
    std::cout << entry.first << ", " << entry.second << std::endl;
  }

  std::cout << std::endl << "Press ENTER to continue..." << std::endl;
  std::cin.get();

  return 0;
}
