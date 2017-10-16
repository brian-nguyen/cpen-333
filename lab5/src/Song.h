/**
 *@file
 *
 * This file contains the definition of a Song in our database
 *
 */

#ifndef LAB4_MUSIC_LIBRARY_SONG_H
#define LAB4_MUSIC_LIBRARY_SONG_H

#include <string>
#include <iostream>

// This "song" class is Immutable: once constructed it cannot be changed.
class Song {
 public:
  const std::string artist;
  const std::string title;

  Song(const std::string artist, const std::string& title) :
      artist(artist), title(title) {}

  std::string toString() const {
    std::string out = artist;
    out.append(" - ");
    out.append(title);
    return out;
  }

  // less-than operator for comparisons, sort by artist then by song
  friend bool operator<(const Song& a, const Song& b) {
    if (a.artist < b.artist) {
      return true;
    } else if (a.artist > b.artist) {
      return false;
    }
    return a.title < b.title;
  }

  // equal-to operator for comparisons, both artist and title must match
  friend bool operator==(const Song& a, const Song& b) {
    return (a.artist == b.artist) && (a.title == b.title);
  }

  // not-equal-to operator for comparisons
  friend bool operator!=(const Song& a, const Song& b) {
    return !(a == b);
  }

  // overloaded stream operator for printing
  //    std::cout << song
  friend std::ostream& operator<<(std::ostream& os, const Song& s) {
    os << s.toString();
    return os;
  }

};

#endif //LAB4_MUSIC_LIBRARY_SONG_H
