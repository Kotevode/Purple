//
// Created by Mark on 22.04.17.
//

#include "DirichletJob.h"

int DirichletJob::get_weight() const {
    return weight;
}

std::ostream &operator<<(std::ostream &os, const DirichletJob &job) {
    os << " offset: " << job.offset << " height: "
       << job.height << " weight: " << job.weight;
    return os;
}

DirichletJob::DirichletJob(const int offset, const int height) : offset(offset), height(height), weight(height) {}
