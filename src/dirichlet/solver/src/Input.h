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
                                                          jobs(jobs),
                                                          f(new double[width * height]) {
            copy(f, f + width * height, this->f.get());
        }

        size_t width, height;
        vector<Evaluation::Job> jobs;
        boost::shared_array<double> f;

    };

}

#endif //PURPLE_DISTRIBUTION_INPUT_H
