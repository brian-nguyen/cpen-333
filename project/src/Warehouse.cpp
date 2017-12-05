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
  computer.init_shelves_and_docks();
  computer.load_inventory();
  computer.init_robots();

  char cmd = 0;
  while (!memory->quit) {
    computer.show_menu();

    std::cin >> cmd;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (cmd) {
      case '0': {
        memory->quit = true;
        break;
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
      case '6': {
        computer.test_order_queue();
        break;
      }
      default:
       safe_printf("nani?\n");
    }
  }

  memory.unlink();
  return 0;
}