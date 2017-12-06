#include <iostream>
#include <cpen333/process/socket.h>

#include "common.h"
#include "Message.h"
#include "WarehouseAPI.h"
#include "Order.h"

void print_menu() {
  std::cout << "1: Order test\n";
  std::cout << "2: Query test\n";
  std::cout << std::endl;
}

// add an order to remote server
void do_order(WarehouseApi &api) {
  Order o(0, 0);
  Product p("banana", 2.2);
  o.add(p, 2);

  // send message to server and wait for response
  OrderMessage msg(o);
  if (api.sendMessage(msg)) {
    // get response
    std::unique_ptr<Message> msgr = api.recvMessage();
    OrderResponse& resp = (OrderResponse&)(*msgr);

    if (resp.status == "OK") {
      std::cout << std::endl << "Success" << std::endl;
    } else {
      std::cout << std::endl << "Fail" << std::endl;
    }
  }

  std::cout << std::endl;
}

// query stock of product
void do_query(WarehouseApi &api) {
  Product p("banana", 2.2);
  
  // send message to server and wait for response
  QueryMessage msg(p);
  if (api.sendMessage(msg)) {
    // get response
    std::unique_ptr<Message> msgr = api.recvMessage();
    QueryResponse& resp = (QueryResponse&)(*msgr);

    if (resp.status == "OK") {
      std::cout << std::endl << "Success" << std::endl;
    } else {
      std::cout << std::endl << "Fail" << std::endl;
    }
  }

  std::cout << std::endl;
}

int main() {
  cpen333::process::socket socket("localhost", PORT);
  std::cout << "Client connecting... ";

  if (socket.open()) {
    std::cout << "SUCCESS\n";

    WarehouseApi api(std::move(socket));
    bool is_exit = false;
    while (!is_exit) {
      print_menu();

      char cmd = 0;
      // get menu entry
      std::cin >> cmd;
      // gobble newline
      std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

      switch (cmd) {
        case '1': {
          do_order(api);
          break;
        }
        case '2': {
          do_query(api);
          break;
        }
        default: {
          is_exit = true;
        }
      }

      cpen333::pause();
    }
  } else {
    std::cout << "FAILED\n";
  }

  return 0;
}