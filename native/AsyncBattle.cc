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

using Risk::Battle;

namespace Risk {

    class AsyncBattle : public AsyncWorker {

    private:
        class Battle& battle;
        int numPlays = 0;

    public:
        explicit AsyncBattle(Callback *callback, Battle& battle, int numPlays) : AsyncWorker(callback), battle(battle), numPlays(numPlays) {}

        ~AsyncBattle() {}

      // Executed inside the worker-thread.
      // It is not safe to access V8, or V8 data structures
      // here, so everything we need for input and output
      // should go on `this`.
        void Execute () {
            this->battle.PlayFor(this->numPlays);
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

};

NAN_METHOD(Battle::PlayAsync) {
    Nan::HandleScope scope;

    Local<Object> battle;
    int numPlays;
    Local<Function> callback;

    if (3 == info.Length()) {
        battle = info[0].As<v8::Object>();
        numPlays = info[1]->Uint32Value();
        callback = info[2].As<v8::Function>();

    } else if (2 == info.Length()) {
        battle = info.Holder();
        numPlays = info[0]->Uint32Value();
        callback = info[1].As<v8::Function>();

    } else {
        battle = info.Holder();
        numPlays = 1;
        callback = info[0].As<v8::Function>();
    }

    AsyncQueueWorker(new AsyncBattle(new Nan::Callback(callback), *Nan::ObjectWrap::Unwrap<Battle>(battle), numPlays));
};

#endif
