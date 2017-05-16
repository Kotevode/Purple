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
#include "../src/evaluation/Section.h"

using namespace Dirichlet;
using namespace Evaluation;

TEST(EvaluationTests, can_serialize_section) {
    // TODO: Do test with memorystream
    const string tempfile = "temp";
    {
        double *mesh = new double[100];
        fill(mesh, mesh + 100, 1);
        Section s(10, 100, mesh);
        ofstream f(tempfile);
        boost::archive::binary_oarchive ar(f);
        ASSERT_NO_THROW(ar << s);
        f.close();
    }
    {
        Section s;
        ifstream f(tempfile);
        boost::archive::binary_iarchive ar(f);
        ASSERT_NO_THROW(ar >> s);
        ASSERT_EQ(s.start, 10);
        ASSERT_EQ(s.size, 100);
        for(int i = 0; i < s.size; i++)
            ASSERT_EQ(s.mesh[i], 1);
        f.close();
    }
}

TEST(EvaluationTests, can_create_intersection) {
    double mesh[12] = {
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3
    };
    Job a(0, 2, 4);
    Job b(1, 2, 4);
    Section s(mesh, a, b);
    ASSERT_EQ(s.start, 4);
    ASSERT_EQ(s.size, 4);
    for(int i = 0; i < 4; i++)
        ASSERT_EQ(s.mesh[i], 2);
}

TEST(EvaluationTests, can_insert_section) {
    int width = 10;
    int height = 10;
    double *mesh = new double[width * height];
    double *part = new double[width * 3];
    fill(part, part + width * 3, 1);
    Section s(width * 3, width * 3, (const double *)part);
    mesh += s;
    for(double * ins = mesh + width * 3; ins < mesh + width * 6; ins++)
        ASSERT_EQ(*ins, 1);

}

TEST(EvaluationTests, can_evaluate_mesh) {
    ifstream in("test_input_300_1.in");
    auto input = (new StreamParser(in))->parse_input();
    Evaluation::Processor p(input.f, input.u);
    auto job = input.jobs.front();
    while(p.process(job) > 0.001) {}
    ifstream out("test_output_300.out");
    double *ideal = (new StreamParser(out))->parse_mesh(input.height, input.width);
    for (int i = 0; i < input.height * input.width; i++){
        ASSERT_NEAR(ideal[i], p.u[i], 0.01);
    }
}