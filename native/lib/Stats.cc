#ifndef Stats_cc
#define Stats_cc

#include <iostream>
#include <math.h>

namespace Risk {

    using Nan::HandleScope;
    using Nan::ObjectWrap;
    using v8::FunctionCallbackInfo;
    using v8::Value;
    using v8::Local;
    using v8::Object;
    using v8::Function;

    class Stats {

        std::string name;
        long decimals = 10000L;
        long count = 0L;
        long sum = 0L;
        long sum2 = 0L;

    public:
        Stats(std::string name, int decimal) : name(name), decimals(Stats::Pow10(decimal)) {}

        void Add(double num) {
            long _num = (long) num * this->decimals;
            this->count ++;
            this->sum += _num;
            this->sum2 += _num * _num;
        }

        double GetSum() {
            return (double) this->sum / this->decimals;
        }

        double GetSum2() {
            return (double) this->sum2 / ( this->decimals * this->decimals );
        }

        double GetAverage() {
            return this->count > 0 ? (double) this->sum / ( this->count * this->decimals ) : 0.0;
        }

        long GetCount() {
            return this->count;
        }

        double GetStDev() {
            double average = this->GetAverage();
            return this->count > 0 ? sqrt(this->GetSum2() / this->count - average * average) : 0.0;
        }

        void Print() {
            std::cout << this->name << " " << this->GetAverage() << " +/- " << this->GetStDev() << "; count=" << this->GetCount() << std::endl;
        }

        static long Pow10(int pow) {
            long num = 1;
            for (int i = 0; i < pow; i++) {
                num *= 10;
            }
            return num;
        }
    };



    class StatsWrap : public ObjectWrap {

    public:
        Stats* stats;

    public:
        StatsWrap() : stats(NULL) {}

        StatsWrap(Stats* stats) : stats(stats) {};

        StatsWrap(std::string name, int decimals) : stats(new Stats(name, decimals)) {}

        ~StatsWrap() {}

        static StatsWrap* NewInstance(std::string name, int decimals) {
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = { v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), name.c_str()), Nan::New(decimals) };
            v8::Local<v8::Object> instance = Nan::NewInstance(ctor, 2, argv).ToLocalChecked();
            return Nan::ObjectWrap::Unwrap<StatsWrap>(instance);
        }

        static StatsWrap* NewInstance(Stats* stats) {
            return NewInstance(*stats);
        }

        static StatsWrap* NewInstance(Stats& stats) {
            v8::Local<v8::Function> ctor = Nan::New(constructor());
            v8::Local<v8::Value> argv[] = {  };
            v8::Local<v8::Object> instance = Nan::NewInstance(ctor, 0, argv).ToLocalChecked();
            StatsWrap* wrap = Nan::ObjectWrap::Unwrap<StatsWrap>(instance);
            wrap->stats = &stats;
            return wrap;
        }

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> ctor;
            return ctor;
        }

        static NAN_MODULE_INIT(Init) {
            HandleScope scope;

            // define prototype
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            v8::Local<v8::ObjectTemplate> otpl = tpl->InstanceTemplate();
            tpl->SetClassName(Nan::New("Stats").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // define prototype properties
            SetAccessor(otpl, Nan::New("average").ToLocalChecked(), GetAverage);
            SetAccessor(otpl, Nan::New("stdev").ToLocalChecked(), GetStDev);
            SetAccessor(otpl, Nan::New("sum").ToLocalChecked(), GetSum);
            SetAccessor(otpl, Nan::New("count").ToLocalChecked(), GetCount);

            // define prototype methods
            SetPrototypeMethod(tpl, "add", Add);

            // define Stats on exports
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Stats").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        static NAN_METHOD(New) {
            HandleScope scope;

            std::string name;
            if (info[0]->IsString()) {
                name = *v8::String::Utf8Value(info[0]->ToString());
            } else {
                name = "";
            }
            int decimals = info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 1;
            StatsWrap *obj = new StatsWrap(name, decimals);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }

        static NAN_METHOD(NewInstance) {
            HandleScope scope;

            v8::Local<v8::Function> cons = Nan::New(constructor());
            v8::Local<v8::String> name = info[0]->ToString(); //info[0]->IsString() ?  : v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "");
            v8::Local<v8::Integer> decimals = info[1]->ToInteger();
            const int argc = 2;
            v8::Local<v8::Value> argv[argc] = {name, decimals};
            info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
        }

        static NAN_METHOD(Add) {
            HandleScope scope;

            double num = info[0]->NumberValue();
            Unwrap(info)->Add(num);
        }

        static NAN_GETTER(GetAverage) {
            HandleScope scope;
            double average = Unwrap(info)->GetAverage();
            info.GetReturnValue().Set(Nan::New(average));
        }

        static NAN_GETTER(GetStDev) {
            HandleScope scope;
            double stdev = Unwrap(info)->GetStDev();
            info.GetReturnValue().Set(Nan::New(stdev));
        }

        static NAN_GETTER(GetCount) {
            HandleScope scope;
            double count = Unwrap(info)->GetCount();
            info.GetReturnValue().Set(Nan::New(count));
        }

        static NAN_GETTER(GetSum) {
            HandleScope scope;
            double sum = Unwrap(info)->GetSum();
            info.GetReturnValue().Set(Nan::New(sum));
        }

        static Stats* Unwrap(Nan::NAN_METHOD_ARGS_TYPE info) {
            return ObjectWrap::Unwrap<StatsWrap>(info.Holder())->stats;
        }

        static Stats* Unwrap(Nan::NAN_GETTER_ARGS_TYPE info) {
            return ObjectWrap::Unwrap<StatsWrap>(info.Holder())->stats;
        }

    };

}

#endif
