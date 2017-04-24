//
// Created by Mark on 08.03.17.
//

#include "purple/Cluster.h"
#include "efdist.h"

using namespace Purple;

const mpi::communicator &Cluster::get_communicator() const {
    return communicator;
}

void Cluster::distribute(vector<JobInfo> &jobs) const {
    if (communicator.rank() == 0)
        ef_distribution(jobs.begin(), jobs.end(), (size_t) communicator.size());
    broadcast(communicator, jobs, 0);
}

Cluster::~Cluster() {
    delete env;
}
