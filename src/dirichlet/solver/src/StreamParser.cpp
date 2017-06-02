//
// Created by Mark on 24.04.17.
//

#include "StreamParser.h"
#include <boost/shared_array.hpp>

namespace Dirichlet {

    using namespace Evaluation;
    using namespace boost;

    Input StreamParser::parse_input() {
        size_t height, width;

        assert(!stream.eof());

        // Parsing u
        stream >> width;
        stream >> height;
        shared_array<double> u(parse_mesh(width, height));
        shared_array<double> f(parse_mesh(width, height));

        // Parsing jobs
        int job_count;
        stream >> job_count;
        vector<Job> jobs;
        for (int i = 0; i < job_count; i++) {
            int offset;
            int height;
            stream >> offset;
            stream >> height;
            jobs.push_back(Job(offset, height, width));
        }

        return Input(f.get(), u.get(), width, height, jobs);
    }

    double *StreamParser::parse_mesh(size_t width, size_t height) {
        double *result = new double[width * height];
        for (int i = 0; i < width * height; i++)
            stream >> result[i];
        return result;
    }

}