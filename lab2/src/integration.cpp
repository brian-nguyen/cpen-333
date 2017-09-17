#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

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

int main() {

  // YOUR CODE HERE

  return 0;
}