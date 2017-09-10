#include <vector>
#include <iostream>
#include "Car.h"
using namespace std;

int main() {
  vector<Car*> cars(100);

  for (int i = 0; i < cars.size(); i++) {
    if (i < 25) {
      cars[i] = new Prius();
    } else if (25 < i && i < 50) {
      cars[i] = new Mazda3();
    } else if (50 < i && i < 75) {
      cars[i] = new Model3();
    } else {
      cars[i] = new Herbie();
    }
  }

  double dt = 0.1;
  for (int i = 0; i < 100; i++) {
    for (Car * car : cars) {
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