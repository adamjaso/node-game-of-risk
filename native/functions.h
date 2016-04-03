
#ifndef random_h
#define random_h

#include <nan.h>
#include "functions.h"
#include "lib/Random.h"

NAN_METHOD(RandomFunction) {
  double est = Random::next();
  info.GetReturnValue().Set(est);
}

#endif
