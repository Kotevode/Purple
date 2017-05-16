//
// Created by Mark on 25.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H
#define PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H

#include <purple/purple.h>
#include "Job.h"

namespace Dirichlet {
    namespace Evaluation {

        class Processor : public Purple::Processor<Job, double> {

        public:
            Processor(boost::shared_array<const double> f, boost::shared_array<double> u) : f(f), u(u) {}

            virtual double process(Job &job) override;

            boost::shared_array<const double> f;
            boost::shared_array<double> u;
        };

    }
}

#endif //PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H
