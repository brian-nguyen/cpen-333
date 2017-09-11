#include "Car.h"

#include <iostream>
#include <string>
#include "State.h"
#include "Car.h"
using namespace std;

static const double QUARTER_MILE = 402.3;

int main() {

  Car car1("Mazda 3", 1600, 790, 0.61);
  Car car2("Toyota Prius", 1450, 740, 0.58);

  // drive for 60 seconds
  double dt = 0.01;

  // GO!!!!
  car1.accelerate(true);
  car2.accelerate(true);
  for (double t = 0; t <= 60; t += dt) {
    car1.drive(dt);
    car2.drive(dt);

    double p_car1 = car1.getState()->position;
    double p_car2 = car2.getState()->position;

    if (p_car1 >= QUARTER_MILE || p_car2 >= QUARTER_MILE) {
      cout << "Race Over. Winner is..." << endl;
      cout << (p_car1 > p_car2 ? car1 : car2) << endl;
      break;
    }

    cout << "Time: " << t << " ";
    if (p_car1 > p_car2) {
      cout << "Car 1 leading";
    } else if (p_car1 < p_car2) {
      cout << "Car 2 leading";
    } else {
      cout << "Tied";
    }

    cout << endl;
  }

  return 0;
}