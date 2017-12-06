#include <iostream>
#include <cpen333/process/socket.h>

#include "common.h"
#include "safe_printf.h"
#include "Message.h"
#include "WarehouseAPI.h"
#include "Order.h"

void print_menu() {
  std::cout << "---\n1: Order\n";
  std::cout << "2: Query\n";
  std::cout << "---\n";
  std::cout.flush();
}

void print_products() {
  std::cout << "------\n";
  std::cout << "\tEnter a product id:\n";
  std::cout << "\t1. Spoon\n";
  std::cout << "\t2. Fork\n";
  std::cout << "\t3. Knife\n";
  std::cout << "\t4. Chopsticks\n";
  std::cout << "\t5. Tongs\n";
  std::cout << "------\n";
  std::cout.flush();
}

// add an order to remote server
void do_order(WarehouseApi &api) {
  Order o(0, NEW_ORDER);
  bool done = false;
  
  while (!done) {
    std::cout << "\t1: Add Product\n\t2: Place Order\n";
    std::cout.flush();
    char cmd = 0;
    std::cin >> cmd;
    switch (cmd) {
      case '1': {
        print_products();
        char type = 0;
        std::cin >> type;
        char quantity = 0;
        std::cout << "\tEnter a quantity: ";
        std::cin >> quantity;
        switch (type) {
          case '1': {
            Product p("Spoon", 0.0);
            o.add(p, (int)quantity - 48);
            break;
          }
          case '2': {
            Product p("Fork", 0.0);
            o.add(p, (int)quantity - 48);
            break;            
          }
          case '3': {
            Product p("Knife", 0.0);
            o.add(p, (int)quantity - 48);
            break;            
          }
          case '4': {
            Product p("Chopsticks", 0.0);
            o.add(p, (int)quantity - 48);
            break;            
          }
          case '5': {
            Product p("Tongs", 0.0);
            o.add(p, (int)quantity - 48);
            break;            
          }
          default: {
            std::cout << "\tInvalid product\n";
          }
        }
        
        std::cout << "\tOrder contains:\n\n";
        for (const auto& p : o.products_) {
          std::cout << "\t\t" << p.second << " " << p.first.name_ << std::endl;
        }
        std::cout << std::endl;
        break;
      }
      case '2': {
        if (o.products_.size() > 0) {
          done = true;
        } else {
          std::cout << "\tAdd a product\n";
        }
        break;
      }
      default: {
        std::cout << "\tInvalid command\n";
        return;
      }
    }
  }

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
  Product p("asf", 0.0);
  print_products();
  char cmd = 0;
  std::cin >> cmd;
  switch (cmd) {
    case '1': {
      p.name_ = "Spoon";
      break;
    }
    case '2': {
      p.name_ = "Fork";
      break;
    }
    case '3': {
      p.name_ = "Knife";
      break;
    }
    case '4': {
      p.name_ = "Chopsticks";
      break;
    }
    case '5': {
      p.name_ = "Tongs";
      break;
    }
    default: {
      std::cout << "Invalid selection\n";
    }                   
  }

  // send message to server and wait for response
  QueryMessage msg(p);
  if (api.sendMessage(msg)) {
    // get response
    std::unique_ptr<Message> msgr = api.recvMessage();
    QueryResponse& resp = (QueryResponse&)(*msgr);

    if (resp.status == "OK") {
      std::cout << "There are " << resp.quantity << std::endl;
    } else {
      std::cout << std::endl << "Fail" << std::endl;
    }
  }

  std::cout << std::endl;
}

int main() {
  cpen333::process::socket socket("localhost", PORT);
  std::cout << "Client connecting... ";
  std::cout.flush();

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