#include <vector>
#include <iostream>
#include "Car.h"
using namespace std;

int main() {
  vector<Car*> cars(100);

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

  double dt = 0.1;
  for (int i = 0; i < cars.size(); i++) {
    for (Car*& car : cars) {
      Car& c = *car;
      State * s = c.getState();

      if (s->velocity >= 28) {
        c.accelerate(false);
      } else {
        c.accelerate(true);
      }

      c.drive(dt);
      cout << "\nName: " << c.getModel() << "\nPosition: " << s->position << endl;
    }
  }

  for (Car * car : cars) {
    delete car;
  }
    
  return 0;
}