#ifndef Stats_cc
#define Stats_cc

#include <iostream>
#include <math.h>

namespace Risk {

    class Stats {

        long decimals = 10000L;
        long count = 0L;
        long sum = 0L;
        long sum2 = 0L;
        std::string name;

    public:
        Stats(std::string name, int decimal) {
            this->name = name;
            this->decimals = Stats::Pow10(decimal);
        }

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

}

#endif
