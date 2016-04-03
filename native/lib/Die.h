#include <iostream>
#include "Random.h"

#ifndef Die_h
#define Die_h

class Die {

int value = 0;

public:
    Die() {
        this->Roll();
    }

    void Roll() {
        this->value = (int) (Random::next() * 6);
    }

    void Print() {
        std::cout << "value: " << this->value << std::endl;
    }

    int GetValue() {
        return this->value;
    }

};

#endif
