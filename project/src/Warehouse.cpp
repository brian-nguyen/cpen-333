#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cpen333/process/shared_memory.h>
#include <json.hpp>
using JSON = nlohmann::json;

#include "common.h"
#include "Computer.h"
#include "Robot.h"
#include "Product.h"

int main(int argc, char* argv[]) {
  // load warehouse layout
  std::string name = "data/warehouse0.txt";
  if (argc > 1) name = argv[1];

  // initialize shared memory for visualization
  cpen333::process::shared_object<SharedData> memory(SHARED_MEMORY_NAME);

  // start computer and load layout and inventory
  Computer computer;
  computer.load_warehouse(name);
  computer.load_inventory();
  computer.init_robots();

  // some test orders
  Order o1(0, 0);
  Order o2(1, 0);
  Order o3(2, 0);
  std::pair<int, int> g1(3, 1);
  std::pair<int, int> g2(15, 1);
  std::pair<int, int> g3(5, 1);
  std::pair<int, int> g4(15, 5);
  
  std::vector<std::pair<int, int>> goals1;
  std::vector<std::pair<int, int>> goals2;
  std::vector<std::pair<int, int>> goals3;
  
  goals1.push_back(g1);
  goals1.push_back(g2);
  goals1.push_back(g3);
  goals1.push_back(g4);
  o1.set_route(goals1);
  goals2.push_back(g4);
  goals2.push_back(g3);
  goals2.push_back(g2);
  goals2.push_back(g1);
  o2.set_route(goals2);
  goals3.push_back(g3);
  goals3.push_back(g4);
  goals3.push_back(g1);
  goals3.push_back(g2);
  o3.set_route(goals3);

  char cmd = 0;
  while (!memory->quit) {
    computer.show_menu();

    std::cin >> cmd;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (cmd) {
      case '0':
        memory->quit = true;
        break;
      case '1':
        computer.spawn_robot();
        break;
      case '2':
        computer.view_inventory();
        break;
      case '3':
        computer.queue().add(o1);
        computer.queue().add(o2);
        computer.queue().add(o3);
        break;
      default:
       safe_printf("nani?\n");
    }
  }

  memory.unlink();
  return 0;
}