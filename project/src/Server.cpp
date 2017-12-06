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
        std::cout << "Client " << id << " making order " << omsg.order.id_ << std::endl;

        // computer.spawn_robot();
        // computer.test_order_queue();

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

        if (api.sendMessage(QueryResponse(qmsg, "", "OK"))) {
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

    api.recvMessage();
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
  
  // start server
  cpen333::process::socket_server server(PORT);
  if (server.open()) {
    std::cout << "Starting server on port " << PORT << std::endl;
  }

  int id = 0;
  std::mutex mutex;
  cpen333::process::socket client;
  while (true) {
    if (server.accept(client)) {
      WarehouseApi api(std::move(client));

      std::thread thread(service, std::ref(mutex), std::ref(computer), std::move(api), id);
      thread.detach();

      id++;
    }
  }

  server.close();
  memory.unlink();
}