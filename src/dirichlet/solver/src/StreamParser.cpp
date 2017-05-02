//
// Created by Mark on 24.04.17.
//

#include "StreamParser.h"

namespace Dirichlet {

    Input StreamParser::parse_input() {
        int height, width;

        assert(!stream.eof());

        // Parsing u
        stream >> width;
        stream >> height;
        double *u = parse_mesh(width, height);
        double *f = parse_mesh(width, height);

        // Parsing jobs
        int job_count;
        stream >> job_count;
        vector<Job> jobs;
        for (int i = 0; i < job_count; i++) {
            int offset;
            int height;
            stream >> offset;
            stream >> height;
            jobs.push_back(Job(offset, height));
        }

        return Input(u, f, width, height, jobs);
    }

    double * StreamParser::parse_mesh(int width, int height) {
        double *result = new double[width * height];
        for (int i = 0; i < width * height; i++)
            stream >> result[i];
        return result;
    }

}