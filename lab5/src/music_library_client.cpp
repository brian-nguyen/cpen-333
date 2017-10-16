/**
 * @file
 *
 * The Music Library Client connects to a remote server and provides an interface
 * for adding, removing, and searching for songs in the server database
 *
 */

#include "MusicLibrary.h"
#include "JsonMusicLibraryApi.h"

#include <cpen333/process/socket.h>

#include <iostream>
#include <limits>

static const char CLIENT_ADD = '1';
static const char CLIENT_REMOVE = '2';
static const char CLIENT_SEARCH = '3';
static const char CLIENT_QUIT = '4';

// print menu options
void print_menu() {

  std::cout << "=========================================" << std::endl;
  std::cout << "=                  MENU                 =" << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << " (1) Add Song" << std::endl;
  std::cout << " (2) Remove Song" << std::endl;
  std::cout << " (3) Search" << std::endl;
  std::cout << " (4) Quit"  << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << "Enter number: ";
  std::cout.flush();

}

// add a song to remote server
void do_add(MusicLibraryApi &api) {

  std::string artist, title;

  // collect artist and title
  std::cout << std::endl << "Add Song" << std::endl;
  std::cout << "   Artist: ";
  std::getline(std::cin, artist);
  std::cout << "   Title:  ";
  std::getline(std::cin, title);

  // send message to server and wait for response
  Song song(artist, title);
  AddMessage msg(song);
  if (api.sendMessage(msg)) {
    // get response
    std::unique_ptr<Message> msgr = api.recvMessage();
    AddResponseMessage& resp = (AddResponseMessage&)(*msgr);

    if (resp.status == MESSAGE_STATUS_OK) {
      std::cout << std::endl << "   \"" << song << "\" added successfull." << std::endl;
    } else {
      std::cout << std::endl << "   Adding \"" << song << "\" failed: " << resp.info << std::endl;
    }
  }

  std::cout << std::endl;
}

// remove song from server
void do_remove(MusicLibraryApi &api) {

  //=================================================
  // TODO: Implement "remove" functionality
  //=================================================
  
}

// search for songs on server
void do_search(MusicLibraryApi &api) {
  std::string artist_regex, title_regex;

  // collect regular expressions for song search
  std::cout << std::endl << "Search for Songs" << std::endl;
  std::cout << "   Artist Expression: ";
  std::getline(std::cin, artist_regex);
  std::cout << "   Title Expression:  ";
  std::getline(std::cin, title_regex);

  // send search message and wait for response
  SearchMessage msg(artist_regex, title_regex);
  if (api.sendMessage(msg)) {
    // get response
    std::unique_ptr<Message> msgr = api.recvMessage();
    SearchResponseMessage& resp = (SearchResponseMessage&)(*msgr);

    if (resp.status == MESSAGE_STATUS_OK) {
      std::cout << std::endl << "   Results:" << std::endl;
      for (const auto& song : resp.results) {
        std::cout << "      " << song << std::endl;
      }
    } else {
      std::cout << std::endl << "   Search \"" << artist_regex << " - "
                << title_regex << "\" failed: " << resp.info << std::endl;
    }
  }

  std::cout << std::endl;
}

int main() {

  // start client
  cpen333::process::socket socket("localhost", MUSIC_LIBRARY_SERVER_PORT);
  std::cout << "Client connecting...";
  std::cout.flush();

  // if we open the socket successfully, continue
  if (socket.open()) {
    std::cout << "connected." << std::endl;

    // create API handler
    JsonMusicLibraryApi api(std::move(socket));

    // keep reading commands until the user quits
    char cmd = 0;
    while (cmd != CLIENT_QUIT) {
      print_menu();

      // get menu entry
      std::cin >> cmd;
      // gobble newline
      std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

      switch(cmd) {
        case CLIENT_ADD:
          do_add(api);
          break;
        case CLIENT_REMOVE:
          do_remove(api);
          break;
        case CLIENT_SEARCH:
          do_search(api);
          break;
        case CLIENT_QUIT:
          break;
        default:
          std::cout << "Invalid command number " << cmd << std::endl << std::endl;
      }

      cpen333::pause();
    }
  } else {
    std::cout << "failed." << std::endl;
  }

  return 0;
}