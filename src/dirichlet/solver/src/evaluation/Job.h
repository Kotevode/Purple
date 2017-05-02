//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_JOB_H
#define PURPLE_DISTRIBUTION_EVALUATION_JOB_H

#include <purple/purple.h>
#include <ostream>

namespace Dirichlet {
    namespace Evaluation {

        class Job : public Purple::Job {

        public:
            Job() : offset(0), height(0), error(0), weight(0) {}

            Job(int offset, int height, double error = 0.001) :
                    offset(offset), height(height), error(error), weight(height / error) {}

            virtual int get_weight() const override { return weight; }

            const double get_error() const { return error; }

            friend std::ostream &operator<<(std::ostream &os, const Job &job) {
                os << "offset: " << job.offset << " height: " << job.height
                   << " weight: " << job.weight << " error: " << job.error;
                return os;
            }

        private:
            friend class Processor;

            PURPLE_SERIALIZE() {
                ar & offset;
                ar & height;
                ar & weight;
                ar & error;
            }

            int offset, height;
            int weight;
            double error;

        };
    }
}


#endif //PURPLE_DISTRIBUTION_EVALUATION_JOB_H
