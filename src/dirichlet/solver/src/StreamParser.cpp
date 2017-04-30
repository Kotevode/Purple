//
// Created by Mark on 24.04.17.
//

#include "StreamParser.h"

namespace Dirichlet {

    Input StreamParser::parse_input() {
        int height, width;
        double *u = nullptr;
        double *f = nullptr;

        assert(!stream.eof());

        // Parsing u
        stream >> width;
        stream >> height;
        u = (double *) malloc(sizeof(double) * width * height);
        for (int i = 0; i < width * height; i++)
            stream >> u[i];
        f = (double *) malloc(sizeof(double) * width * height);
        for (int i = 0; i < width * height; i++)
            stream >> f[i];

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

}