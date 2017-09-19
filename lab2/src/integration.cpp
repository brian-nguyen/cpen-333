#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

double PI = 3.1415926536;
double VOL_SPHERE = 4 / 3 * PI;
enum DensityFn { exponential, line, sphere };

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

double computeIntegral(int nsamples, DensityFn& fn) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  
  std::cout << " - Samples: " << nsamples << std::endl;
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
    double f;
    switch (fn) {
      case exponential:
        f = exp(0 - pow(p.x, 2));
        break;
      case line:
        f = std::abs(p.x + p.y + p.z);
        break;
      case sphere:
        f = pow(p.x - 1, 2) + pow(p.y - 2, 2) + pow(p.z - 3, 2);      
        break;
      default:
        std::cout << "Undefined Function" << std::endl;
        return 0.0;
    }

    sum += VOL_SPHERE * f;
  }

  return sum / nsamples;
}

int main() {
  DensityFn f = exponential;
  std::cout << "exp(-x^2) " << computeIntegral(1000, f) << std::endl << std::endl;

  f = line;
  std::cout << "abs(x + y + z) " << computeIntegral(1000, f) << std::endl << std::endl;
  
  f = sphere;
  std::cout << "(x - 1)^2 + (y - 2)^2 + (z - 3)^2 " << computeIntegral(1000, f) << std::endl << std::endl;

  return 0;
}