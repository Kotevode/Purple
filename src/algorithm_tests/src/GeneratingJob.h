//
// Created by Mark on 05.05.17.
//

#ifndef PURPLE_DISTRIBUTION_DUMMYJOB_H
#define PURPLE_DISTRIBUTION_DUMMYJOB_H

#include <purple/Job.h>
#include <random>

using namespace std;

class WeightGenerator {

public:
    WeightGenerator(int avg, int sd):gen(mt19937(rd())),distribution(new normal_distribution<>(avg, sd)) {}

    int generate_weight() {
        int result = -1;
        while(result < 0)
            result = round(distribution->operator()(gen));
        return result;
    }

private:
    random_device rd;
    mt19937 gen;
    normal_distribution<> *distribution;

};

class GeneratingJob : public Purple::Job {

public:
    GeneratingJob(int avg, int sd):generator(new WeightGenerator(avg, sd)) {}

    virtual int get_weight() const override {
        return generator->generate_weight();
    }

private:
    WeightGenerator * generator;

};

#endif //PURPLE_DISTRIBUTION_DUMMYJOB_H
