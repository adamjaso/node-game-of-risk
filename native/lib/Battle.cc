#ifndef Battle_cc
#define Battle_cc

#include <iostream>
#include "Dice.cc"
#include "Die.cc"
#include "Stats.cc"
#include "Player.cc"

using namespace std;

namespace Risk {

    using Nan::ObjectWrap;
    using Nan::SetAccessor;

    using v8::Local;
    using v8::String;

    class Battle {

    private:
        bool log = false;

    public:
        Player& offense;
        Player& defense;

    public:
        Battle(int off, int def) :
            offense(*new Player("offense", 3, off)),
            defense(*new Player("defense", 2, def)) {
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

    };



    class BattleWrap : public ObjectWrap {
    public:
        Battle* battle;

    public:
        BattleWrap() : battle(NULL) {}

        BattleWrap(Battle* battle) : battle(battle) {}

        BattleWrap(int off, int def) : battle(new Battle(off, def)) {}

        ~BattleWrap() {}

        Battle* GetBattle() {
            return this->battle;
        }

        static BattleWrap* NewInstance(int off, int def) {
            Nan::HandleScope scope;
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = { Nan::New(off), Nan::New(def) };
            v8::Handle<v8::Object> instance = Nan::NewInstance(ctor, 2, argv).ToLocalChecked();
            return Nan::ObjectWrap::Unwrap<BattleWrap>(instance);
        }

        static BattleWrap* NewInstance(Battle& battle) {
            return NewInstance(battle);
        }

        static BattleWrap* NewInstance(Battle* battle) {
            Nan::HandleScope scope;
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = {  };
            v8::Handle<v8::Object> instance = Nan::NewInstance(ctor, 0, argv).ToLocalChecked();
            BattleWrap* wrap = Nan::ObjectWrap::Unwrap<BattleWrap>(instance);
            wrap->battle = battle;
            return wrap;
        }

        // Node.js Addon Boilerplate

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> ctor;
            return ctor;
        }

        static NAN_MODULE_INIT(Init) {
            Nan::HandleScope scope;

            // define prototype
            v8::Handle<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            v8::Local<v8::ObjectTemplate> otpl = tpl->InstanceTemplate();
            tpl->SetClassName(Nan::New("Battle").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // define prototype properties
            SetAccessor(otpl, Nan::New("numDefense").ToLocalChecked(), GetNumDefense);
            SetAccessor(otpl, Nan::New("numOffense").ToLocalChecked(), GetNumOffense);
            SetAccessor(otpl, Nan::New("numDefenseRemaining").ToLocalChecked(), GetNumDefenseRemaining);
            SetAccessor(otpl, Nan::New("numOffenseRemaining").ToLocalChecked(), GetNumOffenseRemaining);
            SetAccessor(otpl, Nan::New("numOffenseWins").ToLocalChecked(), GetNumOffenseWins);
            SetAccessor(otpl, Nan::New("numDefenseWins").ToLocalChecked(), GetNumDefenseWins);
            SetAccessor(otpl, Nan::New("offense").ToLocalChecked(), GetOffense);
            SetAccessor(otpl, Nan::New("defense").ToLocalChecked(), GetDefense);

            // define prototype methods
            SetPrototypeMethod(tpl, "playAsync", PlayAsync);
            SetPrototypeMethod(tpl, "playSync", PlaySync);
            SetPrototypeMethod(tpl, "setDebug", SetDebug);
            SetPrototypeMethod(tpl, "reset", Reset);

            // define Battle on exports
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Battle").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        static NAN_METHOD(New) {
            Nan::HandleScope scope;

            int offense = info[0]->IsNumber() ? Nan::To<int>(info[0]).FromJust() : 0;
            int defense = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 0;
            BattleWrap *obj = new BattleWrap(offense, defense);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }

        /**
         * Battle.prototype.offense
         */
        static NAN_GETTER(GetOffense) {
            Nan::HandleScope scope;

            BattleWrap* battle = ObjectWrap::Unwrap<BattleWrap>(info.Holder());
            PlayerWrap* wrap = PlayerWrap::NewInstance(battle->battle->offense);
            info.GetReturnValue().Set(wrap->handle());
        }

        /**
         * Battle.prototype.defense
         */
        static NAN_GETTER(GetDefense) {
            Nan::HandleScope scope;

            BattleWrap* battle = ObjectWrap::Unwrap<BattleWrap>(info.Holder());
            PlayerWrap* wrap = PlayerWrap::NewInstance(battle->battle->defense);
            info.GetReturnValue().Set(wrap->handle());
        }

        /**
         * Battle.prototype.numDefense
         */
        static NAN_GETTER(GetNumDefense) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumDefense());
        }

        /**
         * Battle.prototype.numOffense
         */
        static NAN_GETTER(GetNumOffense) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumOffense());
        }

        /**
         * Battle.prototype.numDefenseRemaining
         */
        static NAN_GETTER(GetNumDefenseRemaining) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumDefenseRemaining());
        }

        /**
         * Battle.prototype.numOffenseRemaining
         */
        static NAN_GETTER(GetNumOffenseRemaining) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumOffenseRemaining());
        }

        /**
         * Battle.prototype.numOffenseWins
         */
        static NAN_GETTER(GetNumOffenseWins) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumOffenseWins());
        }

        /**
         * Battle.prototype.numDefenseWins
         */
        static NAN_GETTER(GetNumDefenseWins) {
            Nan::HandleScope scope;
            info.GetReturnValue().Set(Unwrap(info)->GetNumDefenseWins());
        }

        /**
         * Battle.prototype.playSync([numPlays])
         */
        static NAN_METHOD(PlaySync) {
            Nan::HandleScope scope;
            Battle* battle = Unwrap(info);
            int numPlays = info.Length() > 0 ? info[0]->Uint32Value() : 1;
            battle->PlayFor(numPlays);
        }

        /**
         * Battle.prototype.reset()
         */
        static NAN_METHOD(Reset) {
            Nan::HandleScope scope;
            Unwrap(info)->Reset();
        }

        /**
         * Battle.prototype.setDebug([debug])
         */
        static NAN_METHOD(SetDebug) {
            Nan::HandleScope scope;
            bool debug = info.Length() > 0 ? !info[0]->IsFalse() : true;
            Unwrap(info)->SetDebug(debug);
            // info.GetReturnValue().Set(battle);
        }

        /**
         * Battle.prototype.playAsync([numPlays], callback)
         */
        static NAN_METHOD(PlayAsync);



        static Battle* Unwrap(Nan::NAN_METHOD_ARGS_TYPE info) {
            return ObjectWrap::Unwrap<BattleWrap>(info.Holder())->battle;
        }

        static Battle* Unwrap(Nan::NAN_GETTER_ARGS_TYPE info) {
            return ObjectWrap::Unwrap<BattleWrap>(info.Holder())->battle;
        }
    };

}

#endif
