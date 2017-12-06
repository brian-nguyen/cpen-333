#ifndef AMAZOOM_COMMON_H
#define AMAZOOM_COMMON_H

#define PORT 52102

#define WALL_CHAR 'X'
#define DOCK_CHAR '_'
#define EMPTY_CHAR ' '
#define SHELF_CHAR 'S'

#define COL_IDX 0
#define ROW_IDX 1

#define MAX_WAREHOUSE_SIZE 80
#define MAX_ROBOTS 19

#define SHARED_MEMORY_NAME "project_amazoom"
#define SHARED_MUTEX_NAME "amazoom_mutex"
#define WAREHOUSE_MUTEX_NAME "amazoom_warehouse_mutex"

struct WarehouseInfo {
  int rows;
  int cols;
  char warehouse[MAX_WAREHOUSE_SIZE][MAX_WAREHOUSE_SIZE];
};

struct RobotInfo {
  int nrobots;
  int rloc[MAX_ROBOTS][2];
};

struct SharedData {
  WarehouseInfo winfo;
  RobotInfo rinfo;
  bool quit;
};

#endif