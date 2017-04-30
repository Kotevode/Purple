//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_CONJUGATION_PROCESSOR_H
#define PURPLE_DISTRIBUTION_CONJUGATION_PROCESSOR_H

#include <purple/purple.h>
#include "Job.h"
#include "../Result.h"

namespace Dirichlet {
    namespace Conjugation {

        class Processor : public Purple::Processor<Job, Result> {

        public:
            Processor(const int width) : width(width) {}

            virtual Result process(Job &job) override;

        private:
            const int width;

        };

    }
}

#endif //PURPLE_DISTRIBUTION_CONJUGATION_PROCESSOR_H
