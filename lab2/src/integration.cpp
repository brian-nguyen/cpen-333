#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

double PI = 3.1415926536;
double VOL_SPHERE = 4 / 3 * PI;
static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
static std::uniform_real_distribution<double> dist(-1.0, 1.0);

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

class Exponential : public Function {
 public:
  double operator()(double x, double y, double z) {
    double norm2 = x*x+y*y+z*z;
    return std::exp(-norm2);
  }
};

class Line : public Function {
  public:
    double operator()(double x, double y, double z) {
      return std::abs(x + y + z);
    }
};

class Sphere : public Function {
public:
  double operator() (double x, double y, double z) {
    return std::pow(x - 1, 2) + std::pow(y - 2, 2) + std::pow(z - 3, 2);
  }
};

bool isWithinSphere(Point& point) {
  if (std::pow(point.x, 2) + std::pow(point.y, 2) + std::pow(point.z, 2) <= 1) return true;

  return false;
}

Point generateRandomPoint() {
  Point p = { 0, 0, 0};
  do {
    p.x = dist(rnd);
    p.y = dist(rnd);
    p.z = dist(rnd);
  } while (!isWithinSphere(p));

  return p;
}

void summation(std::vector<double>& sums, int idx, int nsamples, Function& fn) {
  for (int i = 0; i < nsamples; i++) {
    Point p = generateRandomPoint();
    double f = fn(p.x, p.y, p.z);
    sums[idx] += VOL_SPHERE * f;
  }
}

double MonteCarloIntegrateThreaded(int nsamples, Function& fn) {
  int nthreads = std::thread::hardware_concurrency();
  std::vector<double> sums(nthreads, 0);
  std::vector<std::thread> threads;

  int msamples = 0;
  for (int i = 0; i < nthreads - 1; i++) {
    threads.push_back(std::thread(summation, std::ref(sums), i, nsamples / nthreads, std::ref(fn)));
    msamples += nsamples / nthreads;
  }
  threads.push_back(std::thread(summation, std::ref(sums), nthreads - 1, nsamples - msamples, std::ref(fn)));

  for (int i = 0; i < nthreads; i++) {
    threads[i].join();
  }

  double integral = 0.0;
  for (int i = 0; i < nthreads; i++) {
    integral += sums[i];
  }

  return integral / nsamples;
}

void computeCenterOfMass(std::vector<double>& c, Function& fn, int& nsamples) {
  XFunction xfn(fn);
  YFunction yfn(fn);
  ZFunction zfn(fn);
  double denominator = MonteCarloIntegrateThreaded(nsamples, fn);
  c[0] = MonteCarloIntegrateThreaded(nsamples, xfn) / denominator;
  c[1] = MonteCarloIntegrateThreaded(nsamples, yfn) / denominator;
  c[2] = MonteCarloIntegrateThreaded(nsamples, zfn) / denominator;
}

long getDuration(std::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::nanoseconds> t1) {
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  long ms = duration_ms.count();
  return ms;
}

int main() {
  int nsamples = 1000000;

  Exponential exponential;
  std::vector<double> c1(3, 0);
  computeCenterOfMass(c1, exponential, nsamples);
  std::cout << "Center of mass: (" << c1[0] << ", " << c1[1] << ", " << c1[2] << ")" << std::endl;

  Line line;
  std::vector<double> c2(3, 0);
  computeCenterOfMass(c2, line, nsamples);
  std::cout << "Center of mass: (" << c2[0] << ", " << c2[1] << ", " << c2[2] << ")" << std::endl;

  Sphere sphere;
  std::vector<double> c3(3, 0);
  computeCenterOfMass(c3, sphere, nsamples);
  std::cout << "Center of mass: (" << c3[0] << ", " << c3[1] << ", " << c3[2] << ")" << std::endl;
  return 0;
}