#ifndef Battle_cc
#define Battle_cc

#include <iostream>
#include "Dice.cc"
#include "Die.cc"
#include "Stats.cc"
#include "Player.cc"

using Nan::ObjectWrap;
using Nan::SetAccessor;

using namespace std;

namespace Risk {

    class Battle : public ObjectWrap {

        bool log = false;

        Player offense;
        Player defense;

    public:
        Battle(int off, int def) : offense("offense", 3, off), defense("defense", 2, def) {
            SetDebug(this->log);
        }

        ~Battle() {}

        void SetDebug(bool debug) {
            this->log = debug;
            this->offense.SetDebug(debug);
            this->defense.SetDebug(debug);
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

        int GetNumOffenseWins() {
            return this->offense.GetNumWins();
        }

        int GetNumDefenseWins() {
            return this->defense.GetNumWins();
        }

        void PlayFor(int nTimes) {
            for (int i = 0; i < nTimes; i++) {
                this->Play();
            }
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

        void PrintStats() {
            std::cout << "offense  wins=" << this->offense.GetNumWins() << "; ";
            this->offense.GetLosses().Print();

            std::cout << "defense  wins=" << this->defense.GetNumWins() << "; ";
            this->defense.GetLosses().Print();
        }

        // Node.js Addon Boilerplate

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> ctor;
            return ctor;
        }

        static NAN_MODULE_INIT(Init) {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            v8::Local<v8::ObjectTemplate> otpl = tpl->InstanceTemplate();
            tpl->SetClassName(Nan::New("Battle").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // define prototype methods
            SetPrototypeMethod(tpl, "playAsync", PlayAsync);
            SetPrototypeMethod(tpl, "playSync", PlaySync);
            SetPrototypeMethod(tpl, "setDebug", SetDebug);
            SetPrototypeMethod(tpl, "reset", Reset);

            // define prototype properties
            SetAccessor(otpl, Nan::New("numDefense").ToLocalChecked(), GetNumDefense);
            SetAccessor(otpl, Nan::New("numOffense").ToLocalChecked(), GetNumOffense);
            SetAccessor(otpl, Nan::New("numDefenseRemaining").ToLocalChecked(), GetNumDefenseRemaining);
            SetAccessor(otpl, Nan::New("numOffenseRemaining").ToLocalChecked(), GetNumOffenseRemaining);
            SetAccessor(otpl, Nan::New("numOffenseWins").ToLocalChecked(), GetNumOffenseWins);
            SetAccessor(otpl, Nan::New("numDefenseWins").ToLocalChecked(), GetNumDefenseWins);

            // define Battle on exports
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

        /**
         * Battle.prototype.numDefense
         */
        static NAN_GETTER(GetNumDefense) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumDefense());
        }

        /**
         * Battle.prototype.numOffense
         */
        static NAN_GETTER(GetNumOffense) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumOffense());
        }

        /**
         * Battle.prototype.numDefenseRemaining
         */
        static NAN_GETTER(GetNumDefenseRemaining) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumDefenseRemaining());
        }

        /**
         * Battle.prototype.numOffenseRemaining
         */
        static NAN_GETTER(GetNumOffenseRemaining) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumOffenseRemaining());
        }

        /**
         * Battle.prototype.numOffenseWins
         */
        static NAN_GETTER(GetNumOffenseWins) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumOffenseWins());
        }

        /**
         * Battle.prototype.numDefenseWins
         */
        static NAN_GETTER(GetNumDefenseWins) {
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            info.GetReturnValue().Set(battle->GetNumDefenseWins());
        }

        /**
         * Battle.prototype.playSync([numPlays])
         */
        static NAN_METHOD(PlaySync) {
            Nan::HandleScope scope;
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            int numPlays = info.Length() > 0 ? info[0]->Uint32Value() : 1;
            battle->PlayFor(numPlays);
        }

        /**
         * Battle.prototype.reset()
         */
        static NAN_METHOD(Reset) {
            Nan::HandleScope scope;
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            battle->Reset();
        }

        /**
         * Battle.prototype.setDebug([debug])
         */
        static NAN_METHOD(SetDebug) {
            Nan::HandleScope scope;
            Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
            bool debug = info.Length() > 0 ? !info[0]->IsFalse() : true;
            battle->SetDebug(debug);
            // info.GetReturnValue().Set(battle);
        }

        /**
         * Battle.prototype.playAsync([numPlays], callback)
         */
        static NAN_METHOD(PlayAsync);

    };

}

#endif
