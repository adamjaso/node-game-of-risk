#ifndef Player_cc
#define Player_cc

#include <nan.h>

using Nan::SetAccessor;
using v8::Local;
using v8::Handle;
using v8::Object;

namespace Risk {

    class Player { //: public Nan::ObjectWrap {
    private:
        static long autoPlayerId;

        bool log = false;

    public:
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
            wins(*new Stats("wins", 1)),
            losses(*new Stats("losses", 4)) {}

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
            this->GetWins().Add(1);
            this->GetLosses().Add(this->numArmiesOriginal - this->numArmies);
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

        Stats& GetWins() {
            return this->wins;
        }

        Dice& GetDice() {
            return this->dice;
        }

        int GetNumWins() {
            return this->GetWins().GetCount();
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

    };

    long Player::autoPlayerId = 0L;



    class PlayerWrap : public ObjectWrap {
    private:
        Player* player;

    public:
        PlayerWrap(Player* player) :
            player(player) {}

        PlayerWrap(std::string name, int numDice, int numArmies) :
            player(new Player(name, numDice, numArmies)) {}

        ~PlayerWrap() {}

        Player* GetPlayer() {
            return this->player;
        }

        StatsWrap* GetLosses() {
            return StatsWrap::NewInstance(this->player->GetLosses());
        }

        static PlayerWrap& NewInstance(std::string name, int numDice, int numArmies) {
            Nan::HandleScope scope;

            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = { v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), name.c_str()), Nan::New(numDice), Nan::New(numArmies) };
            v8::Handle<v8::Object> instance = Nan::NewInstance(ctor, 3, argv).ToLocalChecked();
            return *Nan::ObjectWrap::Unwrap<PlayerWrap>(instance);
        }

        static PlayerWrap* NewInstance(Player* player) {
            return NewInstance(*player);
        }

        static PlayerWrap* NewInstance(Player& player) {
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = {  };
            v8::Local<v8::Object> instance = Nan::NewInstance(ctor, 0, argv).ToLocalChecked();
            PlayerWrap* wrap = Nan::ObjectWrap::Unwrap<PlayerWrap>(instance);
            wrap->player = &player;
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
            tpl->SetClassName(Nan::New("Player").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // define prototype properties
            SetAccessor(otpl, Nan::New("numArmiesOriginal").ToLocalChecked(), GetNumArmiesOriginal);
            SetAccessor(otpl, Nan::New("numArmies").ToLocalChecked(), GetNumArmies);
            SetAccessor(otpl, Nan::New("numWins").ToLocalChecked(), GetNumWins);
            SetAccessor(otpl, Nan::New("dice").ToLocalChecked(), GetDice);

            // define prototype methods
            SetPrototypeMethod(tpl, "reset", Reset);
            SetPrototypeMethod(tpl, "getLosses", GetLosses);
            SetPrototypeMethod(tpl, "roll", Roll);

            // define Player on exports
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Player").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        static NAN_METHOD(New) {
            Nan::HandleScope scope;

            std::string name;
            if (info[0]->IsString()) {
                std::string param1(*v8::String::Utf8Value(info[0]->ToString()));
                name = param1;
            } else {
                name = Player::NextPlayerName();
            }
            int numDice = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 1;
            int numArmies = info[2]->IsNumber() ? Nan::To<int>(info[2]).FromJust() : 1;
            PlayerWrap *obj = new PlayerWrap(name, numDice, numArmies);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }

        static NAN_METHOD(NewInstance) {
            Nan::HandleScope scope;

            v8::Local<v8::Function> cons = Nan::New(constructor());
            v8::Local<v8::String> name = info[0]->IsString() ? info[0]->ToString() : v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), Player::NextPlayerName().c_str());
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
             Nan::HandleScope scope;
             info.GetReturnValue().Set(Unwrap(info)->numArmiesOriginal);
         }

         /**
          * Player.prototype.numArmies
          */
          static NAN_GETTER(GetNumArmies) {
              Nan::HandleScope scope;
              int numArmies = Unwrap(info)->numArmies;
              info.GetReturnValue().Set(numArmies);
          }

          /**
           * Player.prototype.numWins
           */
          static NAN_GETTER(GetNumWins) {
              Nan::HandleScope scope;
              int numWins = Unwrap(info)->GetNumWins();
              info.GetReturnValue().Set(numWins);
          }

          /**
           * Player.prototype.getLosses()
           */
          static NAN_METHOD(GetLosses) {
              Nan::HandleScope scope;
              PlayerWrap* player = ObjectWrap::Unwrap<PlayerWrap>(info.Holder());
              StatsWrap* wrap = StatsWrap::NewInstance(player->GetPlayer()->GetLosses());
              info.GetReturnValue().Set(wrap->handle());
          }

          static NAN_GETTER(GetDice) {
              Nan::HandleScope scope;
              Player* player = Unwrap(info);
              Dice& dice = player->GetDice();
              int numDice = dice.GetNumUsable();

              v8::Handle<v8::Array> array = v8::Array::New(v8::Isolate::GetCurrent(), numDice);
              for (int i = 0; i < numDice; i++) {
                  int value = dice.GetDie(i).GetValue();
                  array->Set(i, Nan::New(value));
              }
              info.GetReturnValue().Set(array);
          }

          /**
           * Player.prototype.reset()
           */
          static NAN_METHOD(Reset) {
              Nan::HandleScope scope;
              Unwrap(info)->Reset();
          }

          static NAN_METHOD(Roll) {
              Nan::HandleScope scope;
              Unwrap(info)->Roll();
          }



          static Player* Unwrap(Nan::NAN_METHOD_ARGS_TYPE info) {
              return ObjectWrap::Unwrap<PlayerWrap>(info.Holder())->player;
          }

          static Player* Unwrap(Nan::NAN_GETTER_ARGS_TYPE info) {
              return ObjectWrap::Unwrap<PlayerWrap>(info.Holder())->player;
          }
    };
}

#endif
