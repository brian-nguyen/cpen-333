#include <iostream>
#include <mutex>
#include <thread>
#include <string>

#include <cpen333/process/socket.h>
#include <cpen333/process/shared_memory.h>

#include "common.h"
#include "Message.h"
#include "WarehouseAPI.h"
#include "Computer.h"

void service(std::mutex& mutex, Computer& computer, WarehouseApi&& api, int id) {
  std::cout << "Client " << id << " connected\n";
  bool is_exit = false;

  std::unique_ptr<Message> msg = api.recvMessage();
  while (!is_exit) {
    switch (msg->type()) {
      case ORDER_MSG: {
        OrderMessage& omsg = (OrderMessage &) (*msg);
        std::cout << "Client " << id << " making order\n";

        {
          std::lock_guard<decltype(mutex)> lock(mutex);
          computer.add_order(omsg.order);
        }

        if (api.sendMessage(OrderResponse(omsg, "", "OK"))) {
          std::cout << "Success\n";
        } else {
          std::cout << "Fail\n";
        }
        break;
      }
      case QUERY_MSG: {
        QueryMessage& qmsg = (QueryMessage &) (*msg);
        std::cout << "Client " << id << " querying for " << qmsg.product.name_ << std::endl;

        int quantity = 0;
        {
          std::lock_guard<decltype(mutex)> lock(mutex);          
          quantity = computer.view_stock(qmsg.product.name_);
        }

        if (api.sendMessage(QueryResponse(qmsg, "", "OK", quantity))) {
          std::cout << "Success\n";
        } else {
          std::cout << "Fail\n";
        }
        break;
      }
      default: {
        std::cout << "Client " << id << " send invalid message\n";
      }
    }

    msg = api.recvMessage();
  }

}

void receive_connections(cpen333::process::socket_server& server, std::mutex& mutex, Computer& computer) {
  int id = 0;
  cpen333::process::socket client;
  while (true) {
    if (server.accept(client)) {
      WarehouseApi api(std::move(client));      
      std::thread thread(service, std::ref(mutex), std::ref(computer), std::move(api), id);
      thread.detach();

      id++;
    }
  }
}

int main(int argc, char* argv[]) {
  // load warehouse layout
  std::string name = "data/warehouse0.txt";
  if (argc > 1) name = argv[1];

  // initialize shared memory for visualization
  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);

  // initialize computer and load inventory
  std::cout << "Starting computer...\n";
  Computer computer;
  computer.load_warehouse(name);
  computer.init_shelves_and_docks();
  computer.load_inventory();
  computer.init_robots();

  // spawn set number of robots
  // for (int i = 0; i < 1; i++) {
    computer.spawn_robot();
  // }
  
  // start server
  cpen333::process::socket_server server(PORT);
  if (server.open()) {
    std::cout << "Starting server on port " << PORT << std::endl;
  } else {
    return 0;
  }

  int id = 0;
  std::mutex mutex;
  cpen333::process::socket client;

  // thread to handle incoming connections
  std::thread thread(receive_connections, std::ref(server), std::ref(mutex), std::ref(computer));

  char cmd = 0;
  while (!memory->quit) {
    computer.show_menu();

    std::cin >> cmd;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (cmd) {
      case '0': {
        memory->quit = true;
      }
      case '1': {
        computer.spawn_robot();
        break;
      }
      case '2': {
        computer.view_inventory();
        break;
      }
      case '3': {
        safe_printf("Enter a product name: ");
        char input[100];
        std::cin.getline(input, sizeof(input));
        std::string str(input);
        safe_printf("Searching for %s\n", str.c_str());
        if (computer.view_stock(str) == -1) {
          safe_printf("\tProduct does not exist\n");
        } else {
          safe_printf("\t%d %s\n", computer.view_stock(str), str.c_str());
        }
        break;
      }
      case '4': {
        safe_printf("Enter a status: ");
        char s = 0;
        std::cin >> s;
        computer.view_orders((int)s);
        break;
      }
      case '5': {
        computer.view_shelves();
        break;
      }
      case '8': {
        computer.arrive(DELIVERY_TRUCK);
        break;
      }
      // case '9': {
      //   computer.arrive(INVENTORY_TRUCK);        
      //   break;
      // }
      case '9': {
        computer.view_low_stock();
        break;
      }
      default: {
        safe_printf("Invalid or disabled command\n");
      }
    }
  }

  server.close();
  memory.unlink();
}