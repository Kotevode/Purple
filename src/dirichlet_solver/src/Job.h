//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_DIRICHLETJOB_H
#define PURPLE_DISTRIBUTION_DIRICHLETJOB_H

#include <purple/purple.h>
#include <ostream>

namespace Dirichlet {

    class Job : public Purple::Job {

    public:
        Job(const int offset, const int height, const double error = 0.001) :
                offset(offset), height(height), error(error), weight(height / error) {}

        virtual int get_weight() const override;

        const double get_error() const { return error; }

        friend std::ostream &operator<<(std::ostream &os, const Job &job);

    private:
        friend class Processor;

        const int offset, height;
        const int weight;
        const double error;

    };

}


#endif //PURPLE_DISTRIBUTION_DIRICHLETJOB_H
