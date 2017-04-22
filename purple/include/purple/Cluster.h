//
// Created by Mark on 08.03.17.
//

#ifndef PURPLE_CLUSTER_H
#define PURPLE_CLUSTER_H

#include <stdio.h>
#include "Job.h"
#include "JobInfo.h"
#include "Processor.h"
#include <boost/mpi.hpp>
#include <vector>

namespace Purple {

    class Cluster {

    public:
        Cluster(int argc = 0, char *argv[] = nullptr) : env(new boost::mpi::environment(argc, argv)) {};

        ~Cluster();

        template<class job_type, class result_type>
        std::vector<result_type>
        process(std::vector<job_type> &jobs, Processor<job_type, result_type> &processor) const;

        const mpi::communicator &get_communicator() const;

    private:
        void distribute(std::vector<JobInfo> &jobs) const;

        boost::mpi::communicator communicator;
        boost::mpi::environment *env;
    };

}

#include "cluster_tdef.cpp"

#endif //PURPLE_CLUSTER_H
