#include "Menu.h"
#include "Chef.h"
#include "Customer.h"
#include "Server.h"
#include "SingleOrderQueue.h"
#include "CircularOrderQueue.h"
#include "DynamicOrderQueue.h"

/**
 * Main function to run the restaurant
 * @return
 */
int main() {
  Menu menu;
  menu.load("./data/maenam.json");

  // bunch of chefs, customers and servers
  std::vector<Chef*> chefs;
  std::vector<Customer*> customers;
  std::vector<Server*> servers;

  const int nchefs = 4;
  const int nservers = 2;
  const int ncustomers = 30;

  //============================================
  // TODO: Change queue types to test all three
  //    - SingleOrderQueue
  //    - CircularOrderQueue
  //    - DynamicOrderQueue
  //============================================
  SingleOrderQueue order_queue;
  SingleOrderQueue serve_queue;

  for (int i=0; i<nchefs; ++i) {
    chefs.push_back(new Chef(i, order_queue, serve_queue));
  }

  for (int i=0; i<nservers; ++i) {
    servers.push_back(new Server(i, serve_queue, customers));
  }

  for (int i=0; i<ncustomers; ++i) {
    customers.push_back(new Customer(i, menu, order_queue));
  }

  // start everyone
  for (auto& chef : chefs) {
    chef->start();
  }
  for (auto& server : servers) {
    server->start();
  }
  for (auto& customer : customers) {
    customer->start();
  }

  // wait for all customers to leave
  for (auto& customer : customers) {
    customer->join();
  }

  //==================================================
  // TODO: Signal all chefs to leave
  //==================================================

  // wait for all chefs to leave
  for (auto& chef : chefs) {
    chef->join();
  }

  //==================================================
  // TODO: Signal all servers to leave
  //==================================================

  // wait for all servers to leave
  for (auto& server : servers) {
    server->join();
  }

  // free all memory
  for (auto& customer : customers) {
    delete customer;
    customer = nullptr;
  }
  for (auto& chef : chefs) {
    delete chef;
    chef = nullptr;
  }
  for (auto& server : servers) {
    delete server;
    server = nullptr;
  }

  std::cout << "Restaurant is closing" << std::endl;
  cpen333::pause();

  return 0;
}