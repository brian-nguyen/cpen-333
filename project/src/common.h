#define WALL_CHAR 'X'
#define EMPTY_CHAR ' '
#define SHELF_CHAR 'S'

#define MAX_WAREHOUSE_SIZE 80
#define MAX_RUNNERS 50

#define SHARED_MEMORY_NAME "project_amazoom"
#define WAREHOUSE_MUTEX_NAME "amazoom_warehouse_mutex"

struct WarehouseInfo {
  int rows;
  int cols;
  char warehouse[MAX_WAREHOUSE_SIZE][MAX_WAREHOUSE_SIZE];
};

struct RobotInfo {
  int nrobots;
  int rloc[MAX_RUNNERS][2];
};

struct SharedData {
  WarehouseInfo winfo;
  RobotInfo rinfo;
  bool quit;
};