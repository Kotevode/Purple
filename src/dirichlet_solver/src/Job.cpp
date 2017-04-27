//
// Created by Mark on 22.04.17.
//

#include "Job.h"

namespace Dirichlet {

    int Job::get_weight() const {
        return weight;
    }

    std::ostream &operator<<(std::ostream &os, const Job &job) {
        os << " offset: " << job.offset << " height: "
           << job.height << " weight: " << job.weight;
        return os;
    }

}
