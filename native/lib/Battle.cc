#ifndef Battle_cc
#define Battle_cc

#include <iostream>
#include "Dice.cc"
#include "Die.cc"
#include "Stats.cc"
#include "Player.cc"

using Nan::ObjectWrap;

using namespace std;

namespace Risk {

    class Battle : public ObjectWrap {

        bool log = false;

        Player offense;
        Player defense;

    public:
        Battle(int off, int def) : offense("offense", 3, off), defense("defense", 2, def) {}

        ~Battle() {}

        void SetDebug(bool debug) {
            this->log = debug;
        }

        void Reset() {
            this->offense.Reset();
            this->defense.Reset();
        }

        int GetNumOffense() {
            return this->offense.GetNumArmiesOriginal();
        }

        int GetNumDefense() {
            return this->defense.GetNumArmiesOriginal();
        }

        int GetNumOffenseRemaining() {
            return this->offense.GetNumArmies();
        }

        int GetNumDefenseRemaining() {
            return this->defense.GetNumArmies();
        }

        int GetOffenseWins() {
            return this->offense.GetNumWins();
        }

        int GetDefenseWins() {
            return this->defense.GetNumWins();
        }

        void Play() {
            this->Reset();

            if (this->log) {
                cout << "starting play" << endl;
            }

            while (this->GetNumOffenseRemaining() > 1 && this->GetNumDefenseRemaining() > 0) {
                this->offense.Roll();
                this->defense.Roll();

                int oscore = 0;
                int dscore = 0;
                int tscore = 0;

                Dice::score(this->offense.GetDice(), this->defense.GetDice(), oscore, dscore, tscore);

                this->PrintState(oscore, dscore, tscore);

                this->offense.Decrement(dscore + tscore);
                this->defense.Decrement(oscore);

                this->offense.ApplyNumUsableOffense();
                this->defense.ApplyNumUsableDefense();
            }

            if (this->GetNumDefenseRemaining() > this->GetNumOffenseRemaining()) {
                this->defense.Won();
            } else {
                this->offense.Won();
            }

            if (this->log) {
                cout << "ending play" << endl << endl;
            }

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
                    << "  offense: " << this->offense.GetDice().ToString()
                    << "  defense: " << this->defense.GetDice().ToString()
                    << endl;
            }
        }

        void printStats() {
            std::cout << "offense  wins=" << this->offense.GetNumWins() << "; ";
            this->offense.GetLosses().Print();

            std::cout << "defense  wins=" << this->defense.GetNumWins() << "; ";
            this->defense.GetLosses().Print();
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

            // SetPrototypeMethod(tpl, "getNumDefense", GetNumDefense);
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

        // static NAN_METHOD(GetNumDefense) {
        //     Nan::HandleScope scope;
        //     Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
        //     int numDefense = battle->GetNumDefense();
        //     info.GetReturnValue().Set(numDefense);
        // }

        static NAN_METHOD(PlayAsync);

    };

}

#endif
