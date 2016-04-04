#ifndef Dice_cc
#define Dice_cc

#include <iostream>
#include <string>
#include <cstdlib>
#include "Die.cc"
#include "StringBuilder.cc"

namespace Risk {

    class Dice {

    private:
        bool log = false;
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

        void SetDebug(bool debug) {
            this->log = debug;
        }

        void Roll() {
            int numUsable = this->GetNumUsable();
            for (int i = 0; i < numUsable; i++) {
                this->GetDie(i).Roll();
            }
            // std::cout << "sorting " << numUsable << std::endl;
            std::sort(this->dice.begin(), this->dice.begin() + numUsable, Dice::Compare);
        }

        void Print() {
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

        void SetNumUsableOffense(int numArmies) {
            if (numArmies > 3) {
                this->SetNumUsable(3);

            } else if (numArmies > 2) {
                this->PrintOffenseDiceChange(2);
                this->SetNumUsable(2);

            } else if (numArmies > 1) {
                this->PrintOffenseDiceChange(1);
                this->SetNumUsable(1);

            } else {
                this->PrintOffenseDiceChange(0);
                this->SetNumUsable(0);
            }
        }

        void SetNumUsableDefense(int numArmies) {
            if (numArmies >= 2) {
                this->SetNumUsable(2);

            } else if (numArmies >= 1) {
                this->PrintDefenseDiceChange(1);
                this->SetNumUsable(1);

            } else {
                this->PrintDefenseDiceChange(0);
                this->SetNumUsable(0);
            }
        }

        int GetNumUsable() {
            int numDice = this->GetNumDice();
            return std::min(this->numUsable > -1 ? this->numUsable : numDice, numDice);
        }

        void ResetNumUsable() {
            this->SetNumUsable(this->GetNumDice());
        }

        void PrintOffenseDiceChange(int toNum) {
            if (this->log) {
                std::cout << "  offense " << (toNum + 1) << " -> " << toNum << std::endl;
            }
        }

        void PrintDefenseDiceChange(int toNum) {
            if (this->log) {
                std::cout << "  defense " << (toNum + 1) << " -> " << toNum << std::endl;
            }
        }

        std::string ToString() {
            return this->ToString(this->GetNumUsable());
        }

        std::string ToString(int min) {
            min = std::min(min, this->GetNumUsable());
            StringBuilder builder = StringBuilder::StringBuilder();
            for (int i = 0; i < min; i++) {
                builder.append(" " + std::to_string(this->GetDie(i).GetValue() + 1));
            }
            return builder.str();
        }

        static void score(Dice dice1, Dice dice2, int &score1, int &score2, int &scoreTies) {
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
}

#endif
