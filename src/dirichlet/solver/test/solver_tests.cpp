//
// Created by Mark on 28.04.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/Solver.h"
#include "../src/StreamParser.h"

using namespace Dirichlet;

class SolverTests : public ::testing::Test {

public:
    static std::shared_ptr<Purple::Cluster> cluster;

    virtual void SetUp() {}

    virtual void TearDown() {}

    static void SetUpTestCase() {
        cluster = std::shared_ptr<Purple::Cluster>(new Purple::Cluster(0, NULL));
    }

    static void TearDownTestCase() {}

};

std::shared_ptr<Purple::Cluster> SolverTests::cluster;

TEST_F(SolverTests, can_evaluate) {
    ifstream file("test_input_300_20.in");
    auto input = (new StreamParser(file))->parse_input();
    cout << "Got input" << endl;
    Solver s(cluster);
    auto results = s.evaluate(input.u, input.f, input.width, input.jobs);
    cluster->as_master([&] {
        ASSERT_EQ(results.size(), input.jobs.size());
        auto job_it = input.jobs.begin();
        auto res_it = results.begin();
        while (job_it != input.jobs.end()) {
            ASSERT_LE(res_it->error, job_it->get_error());
            for (int i = 0; i < res_it->height * res_it->width; i++) {
                ASSERT_NO_THROW(res_it->mesh[i]);
            }
            res_it++;
            job_it++;
        }
    });
}

TEST_F(SolverTests, can_conjugate) {
    int count = 10, height = 100, width = 100;
    vector<Result> results;
    for (int i = 0, o = 0; i < count; i++, o += height / 2)
        results.push_back(
                Result(height, width, new double[height * width], 0, o)
        );
    Solver s(cluster);
    auto result = s.conjugate(width, results);
    cluster->as_master([&] {
        ASSERT_EQ(result.height, (int) (height / 2 * (count + 1)));
        ASSERT_EQ(result.offset, 0);
        for (int i = 0; i < result.height * result.width; i++)
            ASSERT_NO_THROW(result.mesh[i]);
    });
}

TEST_F(SolverTests, can_process) {
    ifstream file("test_input_300_20.in");
    auto input = (new StreamParser(file))->parse_input();
    cout << "Got input" << endl;
    Solver s(cluster);
    auto result = s.process(input);
    cluster->as_master([&] {
        ASSERT_EQ(result.offset, 0);
        ASSERT_EQ(result.height, input.height);
        ASSERT_EQ(result.width, input.width);
        ifstream out("../test_output_300.out");
        double *ideal = (new StreamParser(out))->parse_mesh(input.height, input.width);
        for (int i = 0; i < result.height * result.width; i++)
            ASSERT_NEAR(ideal[i], result.mesh[i], 0.01);
    });
}