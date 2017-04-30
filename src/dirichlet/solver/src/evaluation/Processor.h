//
// Created by Mark on 25.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H
#define PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H

#include <purple/purple.h>
#include "Job.h"
#include "../Result.h"

namespace Dirichlet {
    namespace Evaluation {

        class Processor : public Purple::Processor<Job, Result> {

        public:
            Processor(const double *u, const double *f, const int width) : u(u), f(f), width(width) {}

            virtual Result process(Job &job) override;

        private:
            const double *u;
            const double *f;
            const int width;
        };

    }
}

#endif //PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H
