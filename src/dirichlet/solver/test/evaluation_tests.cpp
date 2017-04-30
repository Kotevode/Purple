//
// Created by Mark on 25.04.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/StreamParser.h"
#include "../src/evaluation/Processor.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

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
    int width = 100, height = 100;
    double error = 0.001;
    double *u = (double *)malloc(sizeof(double) * width * height);
    fill_n(u, width * (height - 1), 0);
    fill_n(u + width * (height - 1), height, 1);
    double *f = (double *)malloc(sizeof(double) * width * height);
    Evaluation::Job job(0, height, error);
    Evaluation::Processor p(u, f, width);
    auto result = p.process(job);
    ASSERT_EQ(result.height, height);
    ASSERT_EQ(result.width, width);
    ASSERT_LE(result.error, error);
    for (int i = 0; i < width * height; i++){
        ASSERT_GE(result.mesh[i], 0);
    }
}