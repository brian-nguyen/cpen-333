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

Point generateRandomPoint() {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  Point p = { dist(rnd), dist(rnd), dist(rnd) };

  while (!isWithinSphere(p)) {
    p.x = dist(rnd);
    p.y = dist(rnd);
    p.z = dist(rnd);
  }

  return p;
}

double computeFunction(int fn, Point& p) {
  switch (fn) {
    case exponential:
      return exp(0 - (pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2)));
    case line:
      return std::abs(p.x + p.y + p.z);
    case sphere:
      return pow(p.x - 1, 2) + pow(p.y - 2, 2) + pow(p.z - 3, 2);      
    default:
      return 0.0;
  }
}

double MonteCarloIntegrate(int nsamples, DensityFn& fn) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  
  double sum = 0.0;
  for (int i = 0; i < nsamples; i++) {
    Point p = generateRandomPoint();

    // evaluate the function and
    // approximate integral
    double f = computeFunction(fn, p);
    sum += f;
  }

  return VOL_SPHERE * sum / nsamples;
}

void MonteCarloIntegrate_2(std::vector<double>& c, int& nsamples, int fn, char multiplier) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  for (int i = 0; i < nsamples; i++) {
    Point p = generateRandomPoint();
    c[0] += computeFunction(fn, p);
    switch (multiplier) {
      case 'x':
        c[1] += p.x * c[0];
        break;
      case 'y':
        c[1] += p.y * c[0];
        break;
      case 'z':
        c[1] += p.z * c[0];
        break;
      default:
        return;
    }
  }

  c[0] *= VOL_SPHERE / nsamples;
  c[1] *= VOL_SPHERE / nsamples;
}

void computeCenterOfMass(int nsamples, DensityFn& fn) {
  std::vector<double> cx(2, 0);
  std::vector<double> cy(2, 0);
  std::vector<double> cz(2, 0);

  MonteCarloIntegrate_2(cx, nsamples, fn, 'x');
  MonteCarloIntegrate_2(cy, nsamples, fn, 'y');
  MonteCarloIntegrate_2(cz, nsamples, fn, 'z');

  std::cout << "Center of mass: (" << cx[1] / cx[0] << ", " << cy[1] / cx[0] << ", " << cz[1] / cx[0] << ")";
}

void summation(std::vector<double>& sums, int idx, int nsamples, int fn) {
  for (int i = 0; i < nsamples; i++) {
    Point p = generateRandomPoint();
    double f = computeFunction(fn, p);
    sums[idx] += VOL_SPHERE * f;
  }
}

double MonteCarloIntegrateThreaded(int nsamples, DensityFn& fn) {
  static std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  int nthreads = std::thread::hardware_concurrency();
  std::vector<double> sums(nthreads, 0);
  std::vector<std::thread> threads;

  int msamples = 0;
  for (int i = 0; i < nthreads - 1; i++) {
    threads.push_back(std::thread(summation, std::ref(sums), i, nsamples / nthreads, fn));
    msamples += nsamples / nthreads;
  }
  threads.push_back(std::thread(summation, std::ref(sums), nthreads - 1, nsamples - msamples, fn));

  for (int i = 0; i < nthreads; i++) {
    threads[i].join();
  }

  double integral = 0.0;
  for (int i = 0; i < nthreads; i++) {
    integral += sums[i];
  }

  return integral / nsamples;
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
  DensityFn f = exponential;

  std::cout << std::endl << "exp(-x^2)" << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Sequential - ";
  computeCenterOfMass(nsamples, f);
  std::cout << " - " << getDuration(t1) << std::endl;
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Threaded - " << MonteCarloIntegrateThreaded(nsamples, f) << " - " << getDuration(t1) << std::endl << std::endl;

  f = line;
  std::cout << "abs(x + y + z)" << std::endl;
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Sequential - ";
  computeCenterOfMass(nsamples, f);
  std::cout << " - " << getDuration(t1) << std::endl;
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Threaded - " << MonteCarloIntegrateThreaded(nsamples, f) << " - " << getDuration(t1) << std::endl << std::endl;

  f = sphere;
  std::cout << "(x - 1)^2 + (y - 2)^2 + (z - 3)^2" << std::endl;
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Sequential - ";
  computeCenterOfMass(nsamples, f);
  std::cout << " - " << getDuration(t1) << std::endl;
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Threaded - " << MonteCarloIntegrateThreaded(nsamples, f) << " - " << getDuration(t1) << std::endl << std::endl;
  return 0;
}