#ifndef AsyncBattle_h
#define AsyncBattle_h

#include <nan.h>

using v8::Object;
using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

class AsyncBattle : public AsyncWorker {

private:
    class Battle& battle;
    int numPlays;

public:
    explicit AsyncBattle(Callback *callback, Battle& battle, int numPlays) : AsyncWorker(callback), battle(battle), numPlays(numPlays) {}

    ~AsyncBattle() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
    void Execute () {
        for (int i = 0; i < this->numPlays; i++) {
            this->battle.Play();
        }
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback () {
        HandleScope scope;

        Local<Value> argv[] = {
            Null()
        };

        callback->Call(1, argv);
    }
};

NAN_METHOD(Battle::PlayAsync) {
    Nan::HandleScope scope;
    Battle *battle = Nan::ObjectWrap::Unwrap<Battle>(info.Holder());
    int numPlays = info[0]->Uint32Value();
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

    AsyncQueueWorker(new AsyncBattle(callback, *battle, numPlays));
}

#endif
