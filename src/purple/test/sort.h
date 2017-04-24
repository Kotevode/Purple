//
// Created by Mark on 14.03.17.
//

#ifndef PURPLE_DISTRIBUTION_SORT_H_H
#define PURPLE_DISTRIBUTION_SORT_H_H

#include <vector>
#include <random>
#include <purple/purple.h>

using namespace Purple;
using namespace std;

class SortJob : public Job {

public:
    SortJob() {}

    SortJob(int count) {
        array = vector<int>(count);
        generate(array.begin(), array.end(), rand);
        weight = (int) array.size() * log(array.size());
    }

    vector<int> &get_array() {
        return array;
    }

    virtual int get_weight() const override {
        return weight;
    }

private:
    PURPLE_SERIALIZE() {
        ar & array;
        ar & weight;
    }

    vector<int> array;
    int weight;

};

class SortResult {

public:
    SortResult() {}

    SortResult(const vector<int> &array) : array(array) {}

    const vector<int> &get_array() const {
        return array;
    }

private:
    PURPLE_SERIALIZE() {
        ar & array;
    }

    vector<int> array;

};

class SortProcessor : public Processor<SortJob, SortResult> {

public:
    virtual SortResult process(SortJob &job) override {
        auto array = job.get_array();
        sort(array.begin(), array.end());
        return SortResult(std::move(array));
    }

};

#endif //PURPLE_DISTRIBUTION_SORT_H_H
