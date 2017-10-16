/**
 * @file
 *
 * This file contains the abstraction required for the communication API.
 *
 * This is to help encapsulate the communication details.
 *
 */

#ifndef LAB4_MUSIC_LIBRARY_API_H
#define LAB4_MUSIC_LIBRARY_API_H

#include <memory>
#include "Message.h"

/**
 * Handles communication layer, separating details from the rest of
 * the program.
 */
class MusicLibraryApi {
 public:

  /**
   * Sends a message
   * @param msg message to write
   * @return true if successful, false if error
   */
  virtual bool sendMessage(const Message& msg) = 0;

  /**
   * Reads a message from the socket.  The returned message is
   * contained within a smart pointer to preserve polymorphism
   * and automatically handle freeing of memory resources.
   *
   * @return parsed message, nullptr if an error occurred
   */
  virtual std::unique_ptr<Message> recvMessage() = 0;

};

#endif //LAB4_MUSIC_LIBRARY_API_H
