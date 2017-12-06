#include <iostream>
#include <mutex>
#include <thread>

#include <cpen333/process/socket.h>

#include "common.h"
#include "Message.h"
#include "WarehouseAPI.h"

void service(std::mutex& mutex, WarehouseApi&& api, int id) {
  std::cout << "Client " << id << " connected\n";
  bool is_exit = false;

  std::unique_ptr<Message> msg = api.recvMessage();
  while (!is_exit) {
    int type = msg->type();
    switch (type) {
      case ORDER_MSG: {
        OrderMessage& omsg = (OrderMessage &) (*msg);
        std::cout << "Client " << id << " making order " << omsg.order.id_ << std::endl;

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
      }
      default: {
        std::cout << "Client " << id << " send invalid message\n";
      }
    }

    api.recvMessage();
  }

}

int main() {
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

      std::thread thread(service, std::ref(mutex), std::move(api), id);
      thread.detach();

      id++;
    }
  }

  server.close();
}