#ifndef CAR_H
#define CAR_H

#include <string>
#include <iostream>
#include "State.h"
#include "physics.h"
using namespace std;

class Car {
  private:
    State state;
    string model;
    
  protected:
    double mass;
    double engine_force;
    double drag_area;
    
  public:
    Car(string model, double mass, double engine_force, double drag_area);
    string getModel();    // gets the model name
    double getMass();          // mass of the car
    void accelerate(bool on);  // turn the accelerator on/off
    virtual void drive(double dt);     // drive the car for an amount of time dt
    State * getState();          // returns the car's state
    bool is_accelerating;
};

class Prius : public Car {
  public:
    Prius() : Car("Toyota Prius", 1450, 740, 0.58) { }
};

class Mazda3: public Car {
  public:
    Mazda3() : Car("Mazda 3", 1450, 740, 0.58) { }
};

class Model3: public Car {
  public:
    Model3() : Car("Model 3", 1450, 740, 0.58) { }
};

class Herbie: public Car {
  public:
    Herbie() : Car("Herbie", 1450, 740, 0.58) { }
    void drive(double dt) {
      Car::drive(dt);
      cout << "Herbie Drive" << endl;
    }
};

inline std::ostream& operator<<(std::ostream& os, Car& car) {  
  os << "model: " << car.getModel() << ", mass: " << car.getMass();
  return os;
}

#endif