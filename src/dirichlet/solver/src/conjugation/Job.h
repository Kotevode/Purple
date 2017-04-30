//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_CONJUGATION_JOB_H
#define PURPLE_DISTRIBUTION_CONJUGATION_JOB_H

#include <purple/purple.h>
#include "../Result.h"
#include "Processor.h"

namespace Dirichlet {
    namespace Conjugation {

        class Job : public Purple::Job {

        public:
            Job(const Result &a, const Result &b) : a(a), b(b), weight(a.offset + a.height - b.offset) {
                assert(a.offset < b.offset);
            }

            Job(const Result &a) : a(a), b(Result()), weight(a.height) {}

            virtual int get_weight() const override { return weight; }

        private:
            friend class Processor;

            const Result a, b;
            const int weight;

        };

    }
}

#endif //PURPLE_DISTRIBUTION_CONJUGATION_JOB_H
