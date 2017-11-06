#ifndef MIDTERM_TURTLES_H
#define MIDTERM_TURTLES_H

#define TURTLE_MEMORY_NAME "q4_turtle_memory"
#define TURTLE_MUTEX_NAME  "q4_turtle_mutex"

struct TurtleInfo {
  int checkin;    // checkin count
  int attacked;   // defeat trigger
};

struct TurtleMemory {
  TurtleInfo donatello;
  TurtleInfo leonardo;
  TurtleInfo michelangelo;
  TurtleInfo raphael;
};


#endif //MIDTERM_TURTLES_H
