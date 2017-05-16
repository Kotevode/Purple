//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_JOB_H
#define PURPLE_DISTRIBUTION_EVALUATION_JOB_H

#include <purple/purple.h>
#include <ostream>
#include <algorithm>
#include <boost/shared_array.hpp>

namespace Dirichlet {
    namespace Evaluation {

        struct Job : public Purple::Job {

        public:
            Job() : offset(0), height(0), width(0) {}

            Job(size_t offset, size_t height, size_t width) :
                    offset(offset), height(height), width(width) {
            }

            virtual int get_weight() const override { return height * width; }

            friend std::ostream &operator<<(std::ostream &os, const Job &job) {
                os << "offset: " << job.offset << " height: " << job.height;
                return os;
            }

            PURPLE_SERIALIZE() {
                Purple::Job::serialize(ar, version);
                ar & offset;
                ar & height;
                ar & width;
            }

            size_t offset, height, width;

        };
    }
}


#endif //PURPLE_DISTRIBUTION_EVALUATION_JOB_H
