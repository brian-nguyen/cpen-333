/**
 * @file
 *
 * This contains the data structure for storing the music library locally in memory.
 *
 */
#ifndef LAB4_MUSIC_LIBRARY_H
#define LAB4_MUSIC_LIBRARY_H

#include "Song.h"
#include <vector>
#include <set>
#include <regex>

// Stores a list of songs
class MusicLibrary {
  // private vector
  std::set<Song> songs_;

 public:

  /**
   * Adds a song to the music library
   * @param song song info to add
   * @return true if added, false if already exists
   */
  bool add(const Song& song) {
    // try to add element to the set
    auto elem = songs_.insert(song);
    return elem.second;
  }

  /**
   * Adds songs to the music library
   * @param songs song info to add
   * @return number of songs added
   */
  size_t add(const std::vector<Song>& songs) {
    size_t count = 0;

    for (const Song& song : songs) {
      if (add(song)) {
        ++count;
      }
    }

    return count;
  }

  /**
   * Removes a song from the music library
   * @param song song info to remove
   * @return true if removed, false if not in library
   */
  bool remove(const Song& song) {
    size_t removed = songs_.erase(song);
    return removed > 0;
  }

  /**
   * Removes songs from the music library
   * @param songs song info to remove
   * @return number of songs removed
   */
  size_t remove(const std::vector<Song>& songs) {
    size_t count = 0;

    for (Song s : songs) {
      if (remove(s)) {
        count++;
      }
    }

    return count;
  }

  /**
   * Finds songs in the database matching title and artist expressions
   * @param artist_regex artist regular expression
   * @param title_regex title regular expression
   * @return set of songs matching expressions
   */
  std::vector<Song> find(const std::string& artist_regex,
                         const std::string& title_regex) const {
    std::vector<Song> out;

    // compile regular expressions
    std::regex aregex(artist_regex);
    std::regex tregex(title_regex);

    // search through songs for titles and artists matching search expressions
    for (const auto& song : songs_) {
      if (std::regex_search(song.artist, aregex) && std::regex_search(song.title, tregex)) {
        out.push_back(song);
      }
    }

    return out;
  }

  /**
   * Retrieves the unmodifiable list of songs
   * @return internal set of songs
   */
  const std::set<Song>& songs() const {
    return songs_;
  }
};

#endif //LAB4_MUSIC_LIBRARY_H
