#include <iostream>

#include <exception>
#include "TestException.h"

#include "common.h"
#include "Computer.h"
#include "Order.h"
#include "Robot.h"
#include "DynamicOrderQueue.h"

void before(Computer& c) {
  c.load_warehouse("data/warehouse0.txt");
  c.load_inventory();
  c.init_robots();
}

void testRobotSpawn() {
  Computer c;
  before(c);
  c.spawn_robot();
  
  if (c.robot_count() != 1) {
    throw TestException(std::string("Robot not spawned correctly"));
  }
}

void testRobotSpawnMultiple() {
  Computer c;
  before(c);
  c.spawn_robot();
  c.spawn_robot();
  if (c.robot_count() != 2) {
    throw TestException(std::string("Multiple robots not spawned correctly"));
  }
}

void testRobotMoveToGoal() {
  Computer c;
  before(c);
  c.spawn_robot();
  Robot* r = c.robots().front();
  std::pair<int, int> goal(3, 1);

  if (r->go(goal) != 1) {
    throw TestException(std::string("Failed to reach goal"));
  }
}

void testRobotMoveToGoals() {
  Computer c;
  before(c);
  c.spawn_robot();
  Robot* r = c.robots().front();

  std::pair<int, int> g1(3, 1);
  std::pair<int, int> g2(15, 1);
  std::pair<int, int> g3(5, 1);
  std::pair<int, int> g4(15, 5);
  
  std::vector<std::pair<int, int>> goals;
  goals.push_back(g1);
  goals.push_back(g2);
  goals.push_back(g2);
  goals.push_back(g2);

  for (auto& goal : goals) {
    if (r->go(goal) != 1) {
      throw TestException(std::string("Failed to reach goals"));
    }
  }
}

int main() {
  try {
    testRobotSpawn();
    testRobotSpawnMultiple();
    testRobotMoveToGoal();
    testRobotMoveToGoals();

    std::cout << "All tests passed" << std::endl;
  } catch (TestException& exc) {
    std::cout << exc.what() << std::endl;
  }

  return 0;
}

