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
        GetFunction(New<FunctionTemplate>(Risk::BattleWrap::PlayAsync)).ToLocalChecked());

    Risk::BattleWrap::Init(target);
    Risk::PlayerWrap::Init(target);
    Risk::StatsWrap::Init(target);
}

NODE_MODULE(GameOfRisk, InitAll);

#endif

/*
node(12982,0x7fff7c695000) malloc: *** error for object 0x10170a3f8:
incorrect checksum for freed object - object was probably modified after being freed.
*** set a breakpoint in malloc_error_break to debug
Segmentation fault: 11
*/
