#ifndef Player_cc
#define Player_cc

#include <nan.h>

using Nan::SetAccessor;
using v8::Local;
using v8::Handle;
using v8::Object;

namespace Risk {

    class Player : public Nan::ObjectWrap {
    private:
        static long autoPlayerId;

        bool log = false;

        std::string name;
        int numArmiesOriginal;
        int numArmies;
        Dice dice;
        Stats& wins;
        Stats& losses;

    public:
        Player(std::string name, int numDice, int numArmies) :
            name(name),
            numArmiesOriginal(numArmies),
            numArmies(numArmies),
            dice(numDice),
            wins(*Stats::NewInstance("wins", 1)),
            losses(*Stats::NewInstance("losses", 4)) {}

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

        static std::string NextPlayerName() {
            return "Player #" + std::to_string(++autoPlayerId);
        }

        static Player* NewInstance(std::string name, int numDice, int numArmies) {
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = { v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), name.c_str()), Nan::New(numDice), Nan::New(numArmies) };
            v8::Local<v8::Object> instance = Nan::NewInstance(ctor, 3, argv).ToLocalChecked();
            return Nan::ObjectWrap::Unwrap<Player>(instance);
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

            // define prototype properties
            SetAccessor(otpl, Nan::New("numArmiesOriginal").ToLocalChecked(), GetNumArmiesOriginal);
            SetAccessor(otpl, Nan::New("numArmies").ToLocalChecked(), GetNumArmies);
            SetAccessor(otpl, Nan::New("numWins").ToLocalChecked(), GetNumWins);
            SetAccessor(otpl, Nan::New("losses").ToLocalChecked(), GetLosses);
            SetAccessor(otpl, Nan::New("dice").ToLocalChecked(), GetDice);

            // define prototype methods
            SetPrototypeMethod(tpl, "reset", Reset);

            // define Player on exports
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Player").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        static NAN_METHOD(New) {
            if (info.IsConstructCall()) {
                std::string name;
                if (info[0]->IsString()) {
                    std::string param1(*v8::String::Utf8Value(info[0]->ToString()));
                    name = param1;
                } else {
                    name = NextPlayerName();
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

        static NAN_METHOD(NewInstance) {
            v8::Local<v8::Function> cons = Nan::New(constructor());
            v8::Local<v8::String> name = info[0]->IsString() ? info[0]->ToString() : v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), NextPlayerName().c_str());
            int numDice = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 0;
            int numArmies = info[2]->IsNumber() ? Nan::To<int>(info[2]).FromJust() : 0;
            const int argc = 3;
            v8::Local<v8::Value> argv[argc] = {name, Nan::New(numDice), Nan::New(numArmies)};
            info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
        }

        /**
         * Player.prototype.numArmiesOriginal
         */
         static NAN_GETTER(GetNumArmiesOriginal) {
             info.GetReturnValue().Set(Nan::ObjectWrap::Unwrap<Player>(info.Holder())->numArmiesOriginal);
         }

         /**
          * Player.prototype.numArmies
          */
          static NAN_GETTER(GetNumArmies) {
              info.GetReturnValue().Set(Nan::ObjectWrap::Unwrap<Player>(info.Holder())->numArmies);
          }

          /**
           * Player.prototype.numWins
           */
          static NAN_GETTER(GetNumWins) {
              int numWins = Nan::ObjectWrap::Unwrap<Player>(info.Holder())->GetNumWins();
              info.GetReturnValue().Set(numWins);
          }

          /**
           * Player.prototype.losses
           */
          static NAN_GETTER(GetLosses) {
              Player* player = Nan::ObjectWrap::Unwrap<Player>(info.Holder());
              info.GetReturnValue().Set(player->losses.handle());
          }

          static NAN_GETTER(GetDice) {
                v8::Handle<v8::Array> array = v8::Array::New(v8::Isolate::GetCurrent(), 3);
                if (array.IsEmpty()) {
                    info.GetReturnValue().Set(v8::Handle<v8::Array>());
                    return;
                }
                Player* player = Nan::ObjectWrap::Unwrap<Player>(info.Holder());
                for (int i = 0; i < player->GetDice().GetNumUsable(); i++) {
                    int value = player->GetDice().GetDie(i).GetValue();
                    array->Set(i, Nan::New(value));
                }
                info.GetReturnValue().Set(array);
          }

          /**
           * Player.prototype.reset()
           */
          static NAN_METHOD(Reset) {
              Nan::HandleScope scope;
              Nan::ObjectWrap::Unwrap<Player>(info.Holder())->Reset();
          }

    };

    long Player::autoPlayerId = 0L;
}

#endif
