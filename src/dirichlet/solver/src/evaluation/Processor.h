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
            Processor(shared_array<const double> f) : f(f) {}

            virtual Result process(Job &job) override;

        private:
            shared_array<const double> f;
        };

    }
}

#endif //PURPLE_DISTRIBUTION_EVALUATION_PROCESSOR_H
