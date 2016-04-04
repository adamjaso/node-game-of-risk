#ifndef Player_cc
#define Player_cc

#include <nan.h>

using Nan::SetAccessor;

namespace Risk {

    class Player : public Nan::ObjectWrap {
    private:
        static long autoPlayerId;

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
            numArmiesOriginal(numArmies),
            numArmies(numArmies),
            dice(numDice),
            wins("wins", 1),
            losses("losses", 4) {}

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

        // Node.js Addon Boilerplate

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> ctor;
            return ctor;
        }

        static NAN_MODULE_INIT(Init) {
            // define prototype
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            v8::Local<v8::ObjectTemplate> otpl = tpl->InstanceTemplate();
            tpl->SetClassName(Nan::New("Player").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            SetAccessor(otpl, Nan::New("numArmiesOriginal").ToLocalChecked(), GetNumArmiesOriginal);

            // define Player on exports
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Player").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        static NAN_METHOD(New) {
            if (info.IsConstructCall()) {
                std::string name;
                if (info[0]->IsString()) {
                    std::string param1(*v8::String::Utf8Value(info[0]->ToString()));
                } else {
                    name = ("Player #" + std::to_string(++autoPlayerId));
                }
                int numDice = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 1;
                int numArmies = info[2]->IsNumber() ? Nan::To<int>(info[2]).FromJust() : 1;
                Player *obj = new Player(name, numDice, numArmies);
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
         * Player.prototype.numArmiesOriginal
         */
         static NAN_GETTER(GetNumArmiesOriginal) {
             Player *player = Nan::ObjectWrap::Unwrap<Player>(info.Holder());
             info.GetReturnValue().Set(player->numArmiesOriginal);
         }

    };

    long Player::autoPlayerId = 0L;
}

#endif
