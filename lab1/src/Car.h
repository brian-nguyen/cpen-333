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
    string getModel();
    double getMass();
    double getEngineForce();
    double getDragArea();
    void accelerate(bool on);
    virtual void drive(double dt);
    State * getState();
    bool is_accelerating;
};

class Prius : public Car {
  public:
    Prius() : Car("Toyota Prius", 1450, 740, 0.58) { }
};

class Mazda3: public Car {
  public:
    Mazda3() : Car("Mazda 3", 1600, 800, 0.65) { }
};

class Model3: public Car {
  public:
    Model3() : Car("Model 3", 1300, 1000, 0.40) { }
};

class Herbie: public Car {
  public:
    Herbie() : Car("Herbie", 1000, 1000, 0.01) { }
    void drive(double dt) {
      cout << "HERBIE DRIVING" << endl;;
      Car::drive(dt);
    }
};

inline std::ostream& operator<<(std::ostream& os, Car& car) {  
  os << "model: " << car.getModel() << ", mass: " << car.getMass() << ", engine force: " << car.getEngineForce() << ", drag area: " << car.getDragArea();
  return os;
}

#endif