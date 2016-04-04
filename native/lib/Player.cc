#ifndef Player_cc
#define Player_cc

namespace Risk {

    class Player : public Nan::ObjectWrap {
    private:
        bool log = false;

        std::string name;
        int numArmiesOriginal;
        int numArmies;
        Dice dice;
        Stats wins;
        Stats losses;

    public:
        Player(std::string name, int numDice, int numArmies) :
            name(name),
            numArmies(numArmies),
            numArmiesOriginal(numArmies),
            dice(numDice),
            wins(Stats::Stats("wins", 1)),
            losses(Stats::Stats("losses", 4)) {}

        ~Player() {}

        void SetDebug(bool debug) {
            this->log = debug;
            this->dice.SetDebug(debug);
        }

        void Reset() {
            this->numArmies = this->numArmiesOriginal;
            this->dice.ResetNumUsable();
        }

        void Won() {
            this->wins.Add(1);
            this->losses.Add(this->numArmiesOriginal - this->numArmies);
        }

        void Roll() {
            this->dice.Roll();
        }

        void ApplyNumUsableDefense() {
            this->dice.SetNumUsableDefense(this->numArmies);
        }

        void ApplyNumUsableOffense() {
            this->dice.SetNumUsableOffense(this->numArmies);
        }

        void Decrement(int numArmies) {
            this->numArmies -= numArmies;
        }

        Stats& GetLosses() {
            return this->losses;
        }

        Dice& GetDice() {
            return this->dice;
        }

        int GetNumWins() {
            return this->wins.GetCount();
        }

        int GetNumArmies() {
            return this->numArmies;
        }

        int GetNumArmiesOriginal() {
            return this->numArmiesOriginal;
        }

    };
}

#endif
