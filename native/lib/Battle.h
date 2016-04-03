#ifndef Battle_h
#define Battle_h

#include <iostream>
#include "Dice.h"
#include "Die.h"
#include "Stats.h"
#include "Player.h"

using namespace std;

class Battle : public Nan::ObjectWrap {

private:
    Player offense;
    Player defense;

public:
    explicit Battle(int off, int def) : offense("offense", 3, off), defense("defense", 2, def) {}

    ~Battle() {}

    int GetNumOffenseOriginal() {
        return this->offense.GetNumArmiesOriginal();
    }

    int GetNumDefenseOriginal() {
        return this->defense.GetNumArmiesOriginal();
    }

    int GetNumOffense() {
        return this->offense.GetNumArmies();
    }

    int GetNumDefense() {
        return this->defense.GetNumArmies();
    }

    Player& GetOffense() {
        return this->offense;
    }

    Player& GetDefense() {
        return this->defense;
    }

    void Reset() {
        this->offense.Reset();
        this->defense.Reset();
    }

    bool IsDone() {
      return !(this->offense.HasMoreThan(1) && this->defense.HasMoreThan(0));
    }

    void Play() {
        this->Reset();

        while (!this->IsDone()) {
            Player::RollAndScore(this->offense, this->defense);
        }
    }

    // Node.js Addon Boilerplate

    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> my_constructor;
        return my_constructor;
    }

    static NAN_MODULE_INIT(Init) {
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(Nan::New("Battle").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        SetPrototypeMethod(tpl, "getNumDefense", GetNumDefense);
        SetPrototypeMethod(tpl, "playAsync", PlayAsync);

        constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("Battle").ToLocalChecked(),
            Nan::GetFunction(tpl).ToLocalChecked());
    }

    static NAN_METHOD(New) {
        if (info.IsConstructCall()) {
            int offense = info[0]->IsNumber() ? Nan::To<int>(info[0]).FromJust() : 0;
            int defense = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 0;
            Battle *obj = new Battle(offense, defense);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            const int argc = 1;
            v8::Local<v8::Value> argv[argc] = {info[0]};
            v8::Local<v8::Function> cons = Nan::New(constructor());
            info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
        }
    }

    static NAN_METHOD(GetNumDefense) {
        Nan::HandleScope scope;
        Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
        int numDefense = battle->GetNumDefense();
        info.GetReturnValue().Set(numDefense);
    }

    static NAN_METHOD(PlayAsync);

};

#endif
