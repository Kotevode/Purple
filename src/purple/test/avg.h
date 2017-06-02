//
// Created by Mark on 14.03.17.
//

#ifndef PURPLE_DISTRIBUTION_AVG_H
#define PURPLE_DISTRIBUTION_AVG_H

#include <purple/purple.h>

using namespace Purple;

class AvgJob : public Job {

public:
    AvgJob(int a = 0, int b = 0) : a(a), b(b) {}

    virtual int get_weight() const override { return 1; }

    int get_a() const { return this->a; }

    int get_b() const { return this->b; }

private:
    PURPLE_SERIALIZE() {
        Job::serialize(ar, version);
        ar & a;
        ar & b;
    }

    int a, b;

};

class AvgResult {

public:
    AvgResult(double result = 0) : result(result) {}

    double get_result() const { return this->result; }

private:
    PURPLE_SERIALIZE() {
        ar & result;
    }

    double result;

};

class AvgProcessor : public Processor<AvgJob, AvgResult> {

public:
    virtual AvgResult process(AvgJob &job) override {
        return AvgResult((double) (job.get_a() + job.get_b()) / 2);
    }

};

#endif //PURPLE_DISTRIBUTION_AVG_H
