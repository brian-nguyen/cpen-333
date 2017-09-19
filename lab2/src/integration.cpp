#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

double PI = 3.1415926536;
double VOL_SPHERE = 4 / 3 * PI;

// three-dimensional point
struct Point {
  double x,y,z;
};

// virtual base class for functions
class Function {
 public:
  virtual double operator()(double x, double y, double z) = 0;
};

// computes x*fn(x,y,z)
class XFunction : public Function {
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return x*fn(x,y,z);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function {
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return y*fn(x,y,z);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return z*fn(x,y,z);
  }
};

bool isWithinSphere(Point& point) {
  if (std::pow(point.x, 2) + std::pow(point.y, 2) + std::pow(point.z, 2) <= 1) return true;

  return false;
}

double computeIntegral(int nsamples) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  
  // summation of all function values
  double sum = 0.0;
  for (int i = 0; i < nsamples; i++) {
    // generate a point
    Point p = { dist(rnd), dist(rnd), dist(rnd) };

    // reject and keeping trying until
    // a point is inside volume of sphere
    while (!isWithinSphere(p)) {
      p.x = dist(rnd);
      p.y = dist(rnd);
      p.z = dist(rnd);
    }

    // evaluate the function and
    // approximate integral
    double f = exp(0 - pow(p.x, 2));
    sum += VOL_SPHERE * f;
  }

  return sum / nsamples;
}

int main() {
  std::cout << computeIntegral(1000) << std::endl;
  return 0;
}