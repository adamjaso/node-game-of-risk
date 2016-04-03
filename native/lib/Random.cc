#include <cstdlib>
#include <ctime>

#ifndef Random_cc
#define Random_cc

namespace Risk {

    class Random {
    public:
      static void Init() {
        std::srand(std::time(0));
        Next();
      }

      static double Next() {
        int rand_int = std::rand();
        return (double) rand_int / RAND_MAX;
      }
    };

}

#endif
