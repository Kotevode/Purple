//
// Created by Mark on 25.04.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/StreamParser.h"
#include "../src/Processor.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace Dirichlet;

TEST(ProcessorTests, can_serialize_results) {
    // TODO: Do test with memorystream
    const string tempfile = "temp";
    {
        Result c(100, 100, (double *)malloc(100 * 100 * sizeof(double)), 0.123);
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
        for(int i = 0; i < c.height * c.width; i++)
            ASSERT_NO_THROW(c.mesh[i]);
        f.close();
    }
}

TEST(ProcessorTests, can_process_dirichlet_jobs) {
    ifstream file("input_file.in");
    auto input = (new StreamParser(file))->parse_input();
    cout << "Got input" << endl;
    Processor p(input.u, input.f, input.width);
    cout << "Processor created" << endl;
    Purple::Cluster c(0, NULL);
    cout << "Cluster initialized" << endl;
    auto results = c.process(input.jobs, p);
    cout << "Jobs processed" << endl;
    ASSERT_EQ(results.size(), input.jobs.size());
    auto job_it = input.jobs.begin();
    auto res_it = results.begin();
    while(job_it != input.jobs.end()){
        ASSERT_LE(res_it->error, job_it->get_error());
        for (int i = 0; i < res_it->height * res_it->width; i++){
            ASSERT_NO_THROW(res_it->mesh[i] + 0);
        }
        res_it++;
        job_it++;
    }
}