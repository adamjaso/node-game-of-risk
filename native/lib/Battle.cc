#include <iostream>
#include "Dice.h"
#include "Die.h"
#include "Stats.h"

#ifndef Battle_cc
#define Battle_cc

using Nan::ObjectWrap;

using namespace std;

namespace Risk {

    class Battle : public ObjectWrap {

        bool log = false;

        Player offense;
        Player defense;

    public:
        Battle(int off, int def) : offense("offense", off), defense("defense", def) {}

        ~Battle() {}

        void SetDebug(bool debug) {
            this->log = debug;
        }

        void Reset() {
            this->offense.Reset();
            this->defense.Reset();
        }

        void Play() {
            this->Reset();

            if (this->log) {
                cout << "starting play" << endl;
            }

            while (this->numOffense > 1 && this->numDefense > 0) {
                this->offense.Roll();
                this->defense.Roll();

                int oscore = 0;
                int dscore = 0;
                int tscore = 0;

                Dice::score(this->offense, this->defense, oscore, dscore, tscore);

                this->printState(oscore, dscore, tscore);

                this->numOffense -= dscore + tscore;
                this->numDefense -= oscore;

                this->offense.ApplyNumUsableOffense();
                this->defense.ApplyNumUsableDefense();
            }

            if (this->numDefense > this->numOffense) {
                this->defense.Won();
            } else {
                this->offense.Won();
            }

            if (this->log) {
                cout << "ending play" << endl << endl;
            }

        }

        int GetNumOffense() {
            return this->offense.GetNumArmiesOriginal();
        }

        int GetDefenseDice() {
            return this->defense.GetNumArmiesOriginal();
        }

        int GetOffenseWins() {
            return this->owins;
        }

        int GetDefenseWins() {
            return this->dwins;
        }

        void PrintState(int oscore, int dscore, int tscore) {
            if (this->log) {
                int olose = dscore + tscore;
                int dlose = oscore;
                cout
                    << "  offense: " << this->offense.GetNumArmies() << " -> " << (this->offense.GetNumArmies() - olose)
                    << "  defense: " << this->defense.GetNumArmies() << " -> " << (this->defense.GetNumArmies() - dlose)
                    << "  offense loses: " << olose
                    << "  defense loses: " << dlose
                    << "  offense: " << this->offense.GetDice().toString()
                    << "  defense: " << this->defense.GetDice().toString()
                    << endl;
            }
        }

        void printStats() {
            std::cout << "offense  wins=" << this->offense.GetNumWins() << "; ";
            this->offense.GetLosses().Print();

            std::cout << "defense  wins=" << this->defense.GetNumWins() << "; ";
            this->defense.GetLosses().Print();
        }

    };

}

#endif
