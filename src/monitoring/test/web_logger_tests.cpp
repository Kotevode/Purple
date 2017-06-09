//
// Created by Mark on 23.05.17.
//

#include <gtest/gtest.h>
#include <monitoring/web/Logger.h>
#include <purple/Cluster.h>
#include <boost/mpi.hpp>
#include "wait.h"

using namespace boost::mpi;
using namespace Monitoring::Web;

class WebLoggerTests : public ::testing::Test {

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

environment *WebLoggerTests::env;

TEST_F(WebLoggerTests, can_write_to_fifo) {
    Purple::Cluster c(new Logger("/tmp/proc"));
    vector<WaitJob> jobs;
    for(int i = 0; i < 5; i++)
        jobs.push_back(WaitJob(i));
    WaitProcessor p;
    c.process(jobs, p);
}