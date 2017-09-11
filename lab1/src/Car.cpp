#include <string>
#include "Car.h"
#include "State.h"
#include "physics.h"
using namespace std;

Car::Car(string model, double mass, double engine_force, double drag_area) {
    this->model = model;
    this->mass = mass;
    this->engine_force = engine_force;
    this->drag_area = drag_area;
    this->is_accelerating = false;
}

string Car::getModel() {
    return this->model;
}

double Car::getMass() {
    return this->mass;
}

double Car::getEngineForce() {
    return this->engine_force;
}

double Car::getDragArea() {
    return this->drag_area;
}

void Car::accelerate(bool on) {
    this->is_accelerating = on;
}

void Car::drive(double dt) {
    const double DENSITY_AIR = 1.225;
    double a = this->state.acceleration;
    double v = this->state.velocity;
    double p = this->state.position;
    double engine_force = this->is_accelerating ? this->engine_force : 0;
    double drag_force = v > 0 ? DENSITY_AIR * this->drag_area * v * v / 2 : 0;        

    a += physics::compute_acceleration(engine_force - drag_force, this->mass);
    v = physics::compute_velocity(v, a, dt);
    p = physics::compute_position(p, v, dt);
    this->state.set(p, v, a, this->state.time + dt);
}

State * Car::getState() {
    return &(this->state);
}