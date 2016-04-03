#include <iostream>
#include <string>
#include "Die.h"
#include "StringBuilder.h"

#ifndef Dice_h
#define Dice_h

class Dice {

int numUsable = -1;
std::vector<Die> dice;

public:
    Dice(int num) {
        dice.reserve(num);
        for (int i = 0; i < num; i++) {
            Die die = Die::Die();
            this->dice.push_back(die);
        }
    }

    void Roll() {
        int numUsable = this->GetNumUsable();
        for (int i = 0; i < numUsable; i++) {
            this->GetDie(i).Roll();
        }
        // std::cout << "sorting " << numUsable << std::endl;
        std::sort(this->dice.begin(), this->dice.begin() + numUsable, Dice::Compare);
    }

    void print() {
        for (int i = 0; i < this->GetNumUsable(); i++) {
            int value = this->GetDie(i).GetValue();
            std::cout << "d" << i << ": " << value << " ";
        }
        std::cout << std::endl;
    }

    Die& GetDie(int i) {
        return this->dice[i];
    }

    int GetNumDice() {
        return this->dice.size();
    }

    void SetNumUsable(int numUsable) {
        int numDice = this->GetNumDice();
        if (numUsable <= numDice) {
            this->numUsable = numUsable;
        } else {
            std::cout << "num usable must be less than or equal to num dice. numUsable = " << numUsable << " given. numDice = " << numDice << " found." << std::endl;
        }
    }

    int GetNumUsable() {
        int numDice = this->GetNumDice();
        return std::min(this->numUsable > -1 ? this->numUsable : numDice, numDice);
    }

    void ResetNumUsable() {
        this->SetNumUsable(this->GetNumDice());
    }

    std::string toString() {
        return this->toString(this->GetNumUsable());
    }

    std::string toString(int min) {
        min = std::min(min, this->GetNumUsable());
        StringBuilder builder = StringBuilder::StringBuilder();
        for (int i = 0; i < min; i++) {
            builder.append(" " + toString(this->GetDie(i).GetValue() + 1));
        }
        return builder.str();
    }

    static void Score(Dice dice1, Dice dice2, int &score1, int &score2, int &scoreTies) {
        score1 = 0;
        score2 = 0;
        scoreTies = 0;

        int die1 = 0;
        int die2 = 0;
        int numDice = std::min(dice1.GetNumUsable(), dice2.GetNumUsable());

        for (int i = 0; i < numDice; i++) {
            die1 = dice1.GetDie(i).GetValue();
            die2 = dice2.GetDie(i).GetValue();
            if (die1 < die2) {
                score2 ++;
            } else if (die1 > die2) {
                score1 ++;
            } else {
                scoreTies ++;
            }
        }
    }

    static bool Compare(Die die1, Die die2) {
        return die1.GetValue() > die2.GetValue();
    }
};

#endif
