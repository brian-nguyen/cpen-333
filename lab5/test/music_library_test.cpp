#include "TestException.h"

#include <MusicLibrary.h>
#include <JsonConverter.h>

#include <iostream>
#include <fstream>
#include <vector>

/**
 * Tries adding a song to the library, then checks if it
 * exists internally. If successful, the library will contain
 * the added song.
 *
 * @param lib library to modify
 * @param artist artist of song to add
 * @param title title of song to add
 * @throws TestException if failed to add song
 */
void testAddSong(MusicLibrary& lib,
             const std::string& artist, const std::string& title ) {

  Song song = {artist, title};
  lib.add(song);

  // check to make sure song exists in library
  for (const auto& s : lib.songs()) {
    if (song == s) {
      // found it!!
      return;
    }
  }

  // didn't find it, throw exception
  throw TestException(std::string("Song not properly added: ")
                          + song.toString());
}

/**
 * Tries removing a song from the library. If successful, the library
 * will no longer contain the added song.
 *
 * @param lib library to modify
 * @param artist artist of song to remove
 * @param title title of song to remove
 * @throws TestException if failed to remove song
 */
void testRemoveSong(MusicLibrary& lib,
                 const std::string& artist, const std::string& title ) {

  Song song = {artist, title};

  // may want to check if songs exists in first place
  lib.remove(song);

  // check to make sure song exists in library
  for (const auto& s : lib.songs()) {
    if (song == s) {
      // found it, throw exception
      throw TestException(std::string("Song not properly removed: ")
                              + song.toString());
      return;
    }
  }

}

/**
 * Finds songs in the library matching a set of criteria
 * @param lib library to search for songs
 * @param artist_regex artist search regular expression
 * @param title_regex title search regular expression
 * @param expected expected search results
 */
void testFindSongs(const MusicLibrary& lib,
                   const std::string& artist_regex,
                   const std::string& title_regex,
                   const std::vector<Song>& expected) {

  std::vector<Song> results = lib.find(artist_regex, title_regex);

  // check that sizes match
  if (results.size() != expected.size()) {
    std::string msg = "Size of results do not match: ";
    msg.append(std::to_string(results.size()));
    msg.append(" vs ");
    msg.append(std::to_string(expected.size()));
    msg.append("\n\t");
    for (const auto& song : results) {
      msg.append(song.toString());
      msg.append(", ");
    }
    msg.append("\n\t  vs\n\t");
    for (const auto& song : expected) {
      msg.append(song.toString());
      msg.append(", ");
    }
    throw TestException(msg);
  }

  // check that each expected song is in results
  for (const Song& esong : expected) {

    // loop through all results to find esong
    bool found = false;
    for (const Song& rsong : results) {
      if (esong == rsong) {
        found = true;
        break;
      }
    }

    // not found, through exception
    if (!found) {
      std::string msg = "Song not found in results: " + esong.toString();
      msg.append("\n\t");
      for (const auto& song : results) {
        msg.append(song.toString());
        msg.append(", ");
      }
      msg.append("\n\t  vs\n\t");
      for (const auto& song : expected) {
        msg.append(song.toString());
        msg.append(", ");
      }
      throw TestException(msg);
    }

  }
}

void setupLibrary(MusicLibrary& lib) {

  // load  data from files
  std::vector<std::string> filenames = {
      "data/billboard_hot_100.json",
      "data/billboard_greatest_hot_100.json",
      "data/billboard_adult_contemporary.json",
      "data/billboard_adult_pop.json",
      "data/billboard_alternative.json",
      "data/billboard_country.json",
      "data/billboard_electronic.json",
      "data/billboard_jazz.json",
      "data/billboard_r&b.json",
      "data/billboard_rap.json",
      "data/billboard_rock.json",
  };

  // parse from file stream
  for (std::string filename : filenames) {
    std::ifstream fin(filename);
    if (fin.is_open()) {
      JSON j;
      fin >> j;
      std::vector<Song> songs = JsonConverter::parseSongs(j);
      lib.add(songs);
    } else {
      std::cerr << "Failed to open file: " << filename << std::endl;
    }
  }
}

int main() {

  MusicLibrary lib;
  setupLibrary(lib);

  try {

    testAddSong(lib, "Auli'i Cravalho", "How Far I'll Go");
    testRemoveSong(lib, "Auli'i Cravalho", "How Far I'll Go");

    std::vector<Song> expected = {{"Taylor Swift", "...Ready For It?"}};
    testFindSongs(lib, "Taylor", "[rR]eady", expected);

    std::cout << "All tests passed!" << std::endl;
  } catch (TestException& exc) {
    std::cout << exc.what() << std::endl;
  }

  return 0;
}