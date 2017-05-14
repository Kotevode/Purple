//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_SOLVER_H
#define PURPLE_DISTRIBUTION_SOLVER_H

#include "Result.h"
#include "Input.h"
#include "evaluation/Processor.h"
#include <purple/Cluster.h>
#include <memory>

namespace Dirichlet {

    class Solver {

    public:
        Solver(std::shared_ptr<Purple::Cluster> cluster) : cluster(cluster) {}

        Result process(Input &input, double error = 0.001);

        void swap_intersections(vector<Evaluation::Job> &jobs);

        void swap_intersections(Evaluation::Job &a, Evaluation::Job &b);

        Result combine(vector<Result> &results, size_t height, double error);

    private:

        std::shared_ptr<Purple::Cluster> cluster;

    };

}

#endif //PURPLE_DISTRIBUTION_SOLVER_H
