//
// Created by Mark on 28.04.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/Solver.h"
#include "../src/StreamParser.h"
#include "../src/evaluation/Job.h"

using namespace Dirichlet;
using namespace Evaluation;

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

TEST_F(SolverTests, can_swap_sections) {
    size_t width = 4;
    size_t height = 2;
    auto comm = cluster->get_communicator();
    auto rank = comm.rank();
    double *mesh = new double[(comm.size() + 1) * width];
    double *part = mesh + rank * width;

    fill(part, part + height * width, rank);
    vector<Job> jobs;
    for (int i = 0; i < comm.size(); i++) {
        Job j(i, height, width);
        j.set_node_number(i);
        jobs.push_back(j);
    }
    Solver s(cluster);
    s.swap_intersections(jobs, mesh);
    for(int i = 0; i < width; i++) {
        if (rank != 0)
            ASSERT_EQ(part[i], rank - 1);
        if (rank != comm.size() - 1)
            ASSERT_EQ(part[i + width], rank + 1);
    }
    delete[] mesh;
}

TEST_F(SolverTests, can_process) {
    ifstream file("test_input_300_20.in");
    auto input = (new StreamParser(file))->parse_input();
    cout << "Got input" << endl;
    Solver s(cluster);
    auto result = s.process(input, 0.001);
    cluster->as_master([&] {
        ifstream out("test_output_300.out");
        double *ideal = (new StreamParser(out))->parse_mesh(input.height, input.width);
        double max_error = 0;
        for (int i = 0; i < input.height * input.width; i++)
            max_error = max(abs(ideal[i] - result[i]), max_error);
        ASSERT_LE(max_error, 0.01);
    });
}