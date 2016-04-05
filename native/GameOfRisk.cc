#ifndef GameOfRisk
#define GameOfRisk

#include <nan.h>
#include "functions.cc"
#include "lib/Random.cc"
#include "lib/Battle.cc"
#include "lib/Player.cc"
#include "lib/Stats.cc"
#include "AsyncBattle.cc"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;

using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {

    Risk::Random::Init();

    Set(target, New<String>("random").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(RandomFunction)).ToLocalChecked());

    Set(target, New<String>("playAsync").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(Risk::Battle::PlayAsync)).ToLocalChecked());

    Risk::Battle::Init(target);
    Risk::Player::Init(target);
    Risk::Stats::Init(target);
}

NODE_MODULE(GameOfRisk, InitAll);

#endif
