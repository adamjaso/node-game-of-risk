#include <iostream>
#include <random>
#include "Random.h"

#ifndef Die_cc
#define Die_cc

namespace Risk {

    class Die {

        int value = 0;

    public:
        Die() {
            this->Roll();
        }

        void Roll() {
            this->value = (int) (Random::Next() * 6);
        }

        void Print() {
            std::cout << "value: " << this->value << std::endl;
        }

        int GetValue() {
            return this->value;
        }

    };
}

#endif
