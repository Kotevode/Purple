//
// Created by Mark on 21.05.17.
//

#include <gtest/gtest.h>
#include <boost/mpi.hpp>
#include "monitoring/console/Logger.h"
#include <purple/Cluster.h>
#include "wait.h"

using namespace boost::mpi;
using namespace Monitoring::Console;

class ConsoleLoggerTests : public ::testing::Test {

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

environment *ConsoleLoggerTests::env;


TEST_F(ConsoleLoggerTests, can_log_to_console) {
    Purple::Cluster c(new Logger());
    vector<WaitJob> jobs;
    for(int i = 0; i < 10; i++)
        jobs.push_back(WaitJob(i));
    WaitProcessor p;
    c.process(jobs, p);
}