#include <vector>
#include <iostream>
#include "Car.h"
using namespace std;

int main() {
  vector<Car*> cars(4);

  for (int i = 0; i < cars.size(); i++) {
    switch (i % 4) {
      case 0:
        cars[i] = new Prius();
        break;
      case 1:
        cars[i] = new Mazda3();
        break;
      case 2:
        cars[i] = new Model3();
        break;
      case 3:
        cars[i] = new Herbie();
        break;
      default:
        cout << "wtf" << endl;
    }
  }

  // run for 3 minutes
  int steps = 1800;
  double dt = 0.1;
  for (int i = 0; i < steps; i++) {
    for (Car* car : cars) {
      Car& c = *car;
      State * s = c.getState();

      s->velocity >= 28.0 ? c.accelerate(false) : c.accelerate(true);

      c.drive(dt);
      // cout << "\n" << c.getModel() << "\nState: " << *s << endl;
    }

    for (int j = 0; j < 4; j++) {
      Car& c = *cars[j];
      cout << c.getModel() << "\nState: " << *c.getState() << endl << "----------------" << endl;;
    }
    cout << "*******************************************" << endl;
  }

  for (Car * car : cars) {
    delete car;
  }
    
  return 0;
}