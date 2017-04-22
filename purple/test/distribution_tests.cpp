//
// Created by Mark on 28.03.17.
//

#import <gtest/gtest.h>
#include <purple/purple.h>
#include "../src/efdist.h"

using namespace std;
using namespace Purple;

class DummyJob : public Job {

public:

    DummyJob(int weight) : weight(weight) {}

    virtual int get_weight() const override {
        return weight;
    }

private:
    int weight = 0;

};

TEST(distribution_tests, can_distribute_tasks) {
    vector<DummyJob> jobs{
            {1},
            {2},
            {3}
    };

    auto info = vector<JobInfo>(3);
    transform(jobs.begin(), jobs.end(), info.begin(), [](auto &j) -> JobInfo {
        return JobInfo(j, 0);
    });
    ef_distribution(info.begin(), info.end(), 2);

    int load[2];
    memset(load, 0, 2 * sizeof(int));

    for_each(info.begin(), info.end(), [&](auto &i) {
        load[i.get_node_number()] += i.get_weight();
    });

    ASSERT_GT(load[0], 0);
    ASSERT_GT(load[1], 0);
    ASSERT_EQ(load[0], load[1]);
    ASSERT_EQ(load[0] + load[1], 6);

}