#include <nan.h>
#include "lib/Random.h"
#include "lib/Battle.h"
#include "AsyncBattle.h"
#include "functions.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {

    Random::init();

    Set(target, New<String>("random").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(RandomFunction)).ToLocalChecked());

    Battle::Init(target);

}

NODE_MODULE(addon, InitAll);
