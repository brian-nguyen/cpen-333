#ifndef STATE_H
#define STATE_H

#include <iostream>

class State {
  public:
    double position;
    double velocity;
    double acceleration;
    double time;

    State() {
      this->position = 0.0;
      this->velocity = 0.0;
      this->acceleration = 0.0;
      this->time = 0.0;
    }

    State(double position, double velocity, double acceleration, double time) {
      this->position = position;
      this->velocity = velocity;
      this->acceleration = acceleration;
      this->time = time;
    }

    void set(double position, double velocity, double acceleration, double time) {
      this->position = position;
      this->velocity = velocity;
      this->acceleration = acceleration;
      this->time = time;
    }
};

// prints out a State class information
inline std::ostream& operator<<(std::ostream& os, State& state) {  
  os << "t: " << state.time << ", x: " << state.position << ", v: " << state.velocity << ", a: " << state.acceleration;
  return os;
}  

#endif