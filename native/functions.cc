#ifndef functions_cc
#define functions_cc

#include "lib/Random.cc"

NAN_METHOD(RandomFunction) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Risk::Random::Next());
}

#endif
