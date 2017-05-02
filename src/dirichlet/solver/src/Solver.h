//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_SOLVER_H
#define PURPLE_DISTRIBUTION_SOLVER_H

#include "Result.h"
#include "Input.h"
#include "evaluation/Processor.h"
#include "conjugation/Processor.h"
#include <purple/Cluster.h>
#include <memory>

namespace Dirichlet {

    class Solver {

    public:
        Solver(std::shared_ptr<Purple::Cluster> cluster) : cluster(cluster) {}

        Result process(Input &input);

        vector<Result> evaluate(const double *u, const double *f, int width, vector<Evaluation::Job> &jobs);

        Result conjugate(int width, vector<Result> results);

    private:

        std::shared_ptr<Purple::Cluster> cluster;

    };

}

#endif //PURPLE_DISTRIBUTION_SOLVER_H
