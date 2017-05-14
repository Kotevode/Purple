//
// Created by Mark on 25.04.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/StreamParser.h"
#include "../src/evaluation/Processor.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <limits>

using namespace Dirichlet;

TEST(EvaluationTests, can_serialize_results) {
    // TODO: Do test with memorystream
    const string tempfile = "temp";
    {
        Result c(100, 100, (double *)malloc(100 * 100 * sizeof(double)), 0.123, 11);
        ofstream f(tempfile);
        boost::archive::binary_oarchive ar(f);
        ASSERT_NO_THROW(ar << c);
        f.close();
    }
    {
        Result c;
        ifstream f(tempfile);
        boost::archive::binary_iarchive ar(f);
        ASSERT_NO_THROW(ar >> c);
        ASSERT_EQ(c.height, 100);
        ASSERT_EQ(c.width, 100);
        ASSERT_EQ(c.error, 0.123);
        ASSERT_EQ(c.offset, 11);
        for(int i = 0; i < c.height * c.width; i++)
            ASSERT_NO_THROW(c.mesh[i]);
        f.close();
    }
}

TEST(EvaluationTests, can_evaluate_mesh) {
    ifstream in("test_input_300_1.in");
    auto input = (new StreamParser(in))->parse_input();
    Evaluation::Processor p(input.f);
    auto job = input.jobs.front();
    Result result;
    do {
        result = p.process(job);
        job = Evaluation::Job(result.offset, result.height, result.width, result.mesh.get());
    } while (result.error > 0.001);
    ASSERT_EQ(result.height, input.height);
    ASSERT_EQ(result.width, input.width);
    ifstream out("test_output_300.out");
    double *ideal = (new StreamParser(out))->parse_mesh(input.height, input.width);
    for (int i = 0; i < result.height * result.width; i++){
        ASSERT_NEAR(ideal[i], result.mesh[i], 0.01);
    }
}