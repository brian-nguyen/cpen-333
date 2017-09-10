#ifndef PHYSICS_H
#define PHYSICS_H

// 1D physics library
namespace physics {

	inline double compute_position(double x0, double v, double dt) {
		return x0 + v * dt;
	}

	inline double compute_velocity(double v0, double a, double dt) {
		return v0 + a * dt;
	}

	inline double compute_velocity(double x0, double t0, double x1, double t1) {
		return (x1 - x0) / (t1 - t0);
	}

	inline double compute_acceleration(double v0, double t0, double v1, double t1) {
		return (v1 - v0) / (t1 - t0);
	}

	inline double compute_acceleration(double f, double m) {
		return f / m;
	}
}
#endif // PHYSICS_H