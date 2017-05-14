//
// Created by Mark on 24.04.17.
//

#ifndef PURPLE_DISTRIBUTION_INPUT_H
#define PURPLE_DISTRIBUTION_INPUT_H

#include "evaluation/Job.h"
#include <vector>
#include <iostream>
#include <boost/shared_array.hpp>

namespace Dirichlet {

    struct Input {
        Input() : width(0), height(0) {}

        Input(const double f[],
              size_t width,
              size_t height,
              const std::vector<Evaluation::Job> &jobs) : width(width),
                                                          height(height),
                                                          jobs(jobs) {
            double * fx = new double[width * height];
            memcpy(fx, f, sizeof(double) * width * height);
            this->f = shared_array<const double>(fx);
        }

        size_t width, height;
        vector<Evaluation::Job> jobs;
        boost::shared_array<const double> f;

    };

}

#endif //PURPLE_DISTRIBUTION_INPUT_H
