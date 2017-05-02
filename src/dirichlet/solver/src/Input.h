//
// Created by Mark on 24.04.17.
//

#ifndef PURPLE_DISTRIBUTION_INPUT_H
#define PURPLE_DISTRIBUTION_INPUT_H

#include "evaluation/Job.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace Dirichlet::Evaluation;

namespace Dirichlet {

    struct Input {
        Input() : u(nullptr), f(nullptr), width(0), height(0) {}

        Input(double *u, double *f, int width, int height, const vector<Job, allocator<Job>> &jobs) : u(u), f(f),
                                                                                                      width(width),
                                                                                                      height(height),
                                                                                                      jobs(jobs) {}

        double *u;
        double *f;
        int width, height;
        vector<Job> jobs;

        virtual ~Input() {
            if (u)
                free(u);
            if (f)
                free(f);
        }

    };

}

#endif //PURPLE_DISTRIBUTION_INPUT_H
