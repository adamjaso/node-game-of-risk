#ifndef Player_h
#define Player_h

#include "Dice.h"
#include "Stats.h"

class Player : public Nan::ObjectWrap {

private:
    std::string name;
    int numArmiesOriginal;
    int numArmies;
    Dice dice;
    Stats wins;
    Stats losses;

public:
    explicit Player(std::string name, int numDice, int numArmies) :
        name(name),
        dice(numDice),
        wins("wins", 1),
        losses("losses", 4),
        numArmiesOriginal(numArmies),
        numArmies(numArmies) {}

    ~Player() {}

    Dice& GetDice() {
        return this->dice;
    }

    int GetNumArmies() {
        return this->numArmies;
    }

    int GetNumArmiesOriginal() {
        return this->numArmiesOriginal;
    }

    int GetNumWins() {
        return this->wins.GetCount();
    }

    Stats GetLosses() {
        return this->losses;
    }

    bool HasMoreThan(int numArmies) {
        return this->numArmies > numArmies;
    }

    void Reset() {
        this->numArmies = this->numArmiesOriginal;
        this->dice.ResetNumUsable();
    }

    void Decrement(int numArmies) {
        this->numArmies -= numArmies;
    }

    void Won() {
        this->losses.add(this->numArmiesOriginal - this->numArmies);
        this->wins.add(1);
    }

    static void RollAndScore(Player& offense, Player& defense) {
        offense.GetDice().Roll();
        defense.GetDice().Roll();

        int oscore = 0;
        int dscore = 0;
        int tscore = 0;

        Dice::Score(offense.GetDice(), defense.GetDice(), oscore, dscore, tscore);

        offense.Decrement(dscore + tscore);
        defense.Decrement(oscore);

        if (offense.GetNumArmies() > 3) {
            offense.GetDice().SetNumUsable(3);

        } else if (offense.GetNumArmies() > 2) {
            offense.GetDice().SetNumUsable(2);

        } else if (offense.GetNumArmies() > 1) {
            offense.GetDice().SetNumUsable(1);

        } else {
            offense.GetDice().SetNumUsable(0);
        }

        if (defense.GetNumArmies() >= 2) {
            defense.GetDice().SetNumUsable(2);

        } else if (defense.GetNumArmies() >= 1) {
            defense.GetDice().SetNumUsable(1);

        } else {
            defense.GetDice().SetNumUsable(0);
        }

        if (offense.GetNumArmies() > 0) {
            offense.Won();
        } else {
            defense.Won();
        }
    }
};

#endif
