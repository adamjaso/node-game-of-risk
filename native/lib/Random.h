#ifndef Random_h
#define Random_h

#include <cstdlib>
#include <ctime>

class Random {
public:
  static void init() {
    std::srand(std::time(0));
    Random::next();
  }

  static double next() {
    int rand_int = std::rand();
    return (double) rand_int / RAND_MAX;
  }
};

#endif
