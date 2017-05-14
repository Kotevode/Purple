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

TEST_F(SolverTests, can_process) {
    ifstream file("test_input_300_20.in");
    auto input = (new StreamParser(file))->parse_input();
    cout << "Got input" << endl;
    Solver s(cluster);
    auto result = s.process(input, 0.001);
    cluster->as_master([&] {
        ASSERT_EQ(result.offset, 0);
        ASSERT_EQ(result.height, input.height);
        ASSERT_EQ(result.width, input.width);
        ifstream out("test_output_300.out");
        double *ideal = (new StreamParser(out))->parse_mesh(input.height, input.width);
        double max_error = 0;
        for (int i = 0; i < result.height * result.width; i++)
            max_error = max(abs(ideal[i] - result.mesh[i]), max_error);
        ASSERT_LE(max_error, 0.01);
    });
}