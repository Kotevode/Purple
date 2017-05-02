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
            Job() : a(Result()), b(Result()), weight(0) {}

            Job(Result &a, Result &b) : a(a), b(b), weight(a.offset + a.height - b.offset) {
                assert(a.offset < b.offset);
            }

            Job(Result &a) : a(a), b(Result()), weight(a.height) {}

            virtual int get_weight() const override { return weight; }

        private:
            friend class Processor;

            PURPLE_SERIALIZE() {
                ar & a;
                ar & b;
                ar & weight;
            }

            Result a, b;
            int weight;

        };

    }
}

#endif //PURPLE_DISTRIBUTION_CONJUGATION_JOB_H
