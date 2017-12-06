#ifndef AMAZOOM_WAREHOUSE_H
#define AMAZOOM_WAREHOUSE_H

#include <string>

#include <cpen333/process/socket.h>

#include <json.hpp>
using JSON = nlohmann::json;

#include "JsonConverter.h"
#include "Message.h"

class WarehouseApi {
  cpen333::process::socket socket_;
  static const char JSON_ID = 0x55;

  bool sendJSON(JSON& j) {
    // dump to string
    std::string jsonstr = j.dump();

    // encode JSON size, big endian format
    //   (most-significant byte in buff[0])
    char buff[4];
    size_t size = jsonstr.size()+1;           // one for terminating zero
    for (int i = 4; i-- > 0;) {
      // cut off byte and shift size over by 8 bits
      buff[i] = (char)(size & 0xFF);
      size = size >> 8;
    }
    // write contents
    bool success  = socket_.write(buff, 4);   // contents size
    success &= socket_.write(jsonstr);        // contents

    return success;
  }

  bool readString(std::string& str, size_t size) {
    char cbuff[256];

    for (int i = 0; i < (size / 256); i++) {
      if (!socket_.read_all(cbuff, 256)) {
        return false;
      }
      str.append(cbuff);
    }
    
    int last_chunk = size % 256;
    if (last_chunk > 0) {
      if (!socket_.read_all(cbuff, last_chunk)) {
        return false;
      }
      str.append(cbuff);
    }

    return true;
  }

  bool recvJSON(JSON& jout) {
    // receive 4-byte size
    char buff[4];
    if (!socket_.read_all(buff, 4)) {
      return false;
    }

    size_t size = 0;
    for (int i = 0; i < 4; i++) {
      size <<= 8;
      size |= ((size_t)buff[i] & 0xFF);
    }
    std::cout << std::endl;

    // read entire JSON string
    std::string str;
    if (!readString(str, size)) {
      return false;
    }

    // parse JSON
    jout = JSON::parse(str);
    return true;
  }

  WarehouseApi();

 public:
  WarehouseApi(cpen333::process::socket&& socket) : socket_(std::move(socket)) { }

  bool sendMessage(const Message& msg) {
    JSON jmsg = JsonConverter::toJSON(msg);
    std::cout << jmsg.dump() << std::endl;

    char id = JSON_ID;
    if (!socket_.write(&id, 1)) {
      return false;
    }

    return sendJSON(jmsg);;
  }

  std::unique_ptr<Message> recvMessage() {
    char id;
    if (!socket_.read_all(&id, 1) || id != JSON_ID) {
      return nullptr;
    }

    JSON jmsg;
    if (!recvJSON(jmsg)) {
      return nullptr;
    }

    std::cout << jmsg.dump() << std::endl;
    return JsonConverter::parseMessage(jmsg);
  }

};

#endif