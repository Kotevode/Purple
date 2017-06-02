//
// Created by Mark on 14.03.17.
//

#include <gtest/gtest.h>
#include <purple/purple.h>
#include <boost/mpi.hpp>
#include "avg.h"
#include "sort.h"

using namespace Purple;
using namespace std;
using namespace boost::mpi;

class ClusterTests : public ::testing::Test {

public:
    virtual void SetUp() {}

    virtual void TearDown() {}

    static environment *env;

    static void SetUpTestCase() {
        int argc = 0;
        char **argv;
        env = new environment(argc, argv);
    }

    static void TearDownTestCase() {
        delete env;
    }

};

environment *ClusterTests::env;

TEST_F(ClusterTests, test_can_process) {
    vector<AvgJob> jobs{
            AvgJob(5, 5),
            AvgJob(6, 6),
            AvgJob(7, 9),
    };

    AvgProcessor p;
    Cluster c;
    vector<AvgResult> results = c.process(jobs, p);

    if (c.get_communicator().rank() == 0) {
        ASSERT_EQ(results[0].get_result(), 5.0);
        ASSERT_EQ(results[1].get_result(), 6.0);
        ASSERT_EQ(results[2].get_result(), 8.0);
    }
}

TEST_F(ClusterTests, test_can_process_complex_jobs) {
    vector<SortJob> jobs;
    for (int i = 1; i <= 1000; i++) {
        jobs.push_back(SortJob(i));
    }

    SortProcessor p;
    Cluster c;
    vector<SortResult> results = c.process(jobs, p);

    if (c.get_communicator().rank() == 0) {
        ASSERT_EQ(results.size(), 1000);

        int j = 1;
        for_each(results.begin(), results.end(), [&](SortResult &result) {
            ASSERT_EQ(result.get_array().size(), j++);
            ASSERT_TRUE(is_sorted(result.get_array().begin(), result.get_array().end()));
        });
    }
}