#ifndef Die_cc
#define Die_cc

#include <iostream>
#include "Random.cc"

namespace Risk {

    class Die {

    private:
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
