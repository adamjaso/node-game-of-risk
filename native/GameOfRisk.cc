#ifndef GameOfRisk
#define GameOfRisk

#include <nan.h>
#include "functions.cc"
#include "lib/Random.cc"

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
}

NODE_MODULE(GameOfRisk, InitAll);

#endif
