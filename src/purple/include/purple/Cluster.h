//
// Created by Mark on 08.03.17.
//

#ifndef PURPLE_CLUSTER_H
#define PURPLE_CLUSTER_H

#include <stdio.h>
#include <boost/mpi.hpp>
#include <vector>
#include <list>
#include <numeric>
#include "Job.h"
#include "JobInfo.h"
#include "Processor.h"
#include "ResultInfo.h"
#include "efdist.h"

namespace Purple {

    using namespace std;
    namespace mpi = boost::mpi;

    class Cluster {

    public:
        Cluster(int argc = 0, char *argv[] = nullptr) : env(new boost::mpi::environment(argc, argv)) {};

        ~Cluster() {
            delete env;
        }

        template<typename __closure>
        void as_master(__closure closure) {
            if (communicator.rank() == 0)
                closure();
        }

        template<class job_type, class result_type>
        vector<result_type> process(vector<job_type> &jobs, Processor<job_type, result_type> &processor) const {

            // Distributing jobs
            broadcast(communicator, jobs, 0);

            vector<JobInfo> info(jobs.size());
            int i = 0;
            for_each(jobs.begin(), jobs.end(), [&](job_type &j) {
                info[i] = JobInfo(j, i);
                i++;
            });
            distribute(info);

            // Processing

            vector<ResultInfo<result_type> > partial_results;
            for_each(info.begin(), info.end(), [&](auto &info) {
                if (info.get_node_number() != communicator.rank())
                    return;
                partial_results.push_back(
                        ResultInfo<result_type>(processor.process(jobs[info.get_index()]), info.get_index())
                );
            });
            MPI_Barrier(communicator);

            // Gathering results

            vector<vector<ResultInfo<result_type> >> gathered(communicator.size());
            mpi::gather(communicator, partial_results, gathered, 0);

            vector<result_type> results(jobs.size());
            for_each(gathered.begin(), gathered.end(), [&](auto &partial) {
                for_each(partial.begin(), partial.end(), [&](auto &result_info) {
                    results[result_info.get_index()] = std::move(result_info.get_result());
                });
            });

            return results;
        }

        const mpi::communicator &get_communicator() const {
            return this->communicator;
        }

    private:
        void distribute(std::vector<JobInfo> &jobs) const {
            if (communicator.rank() == 0)
                ef_distribution(jobs.begin(), jobs.end(), (size_t) communicator.size());
            broadcast(communicator, jobs, 0);
        }

        boost::mpi::communicator communicator;
        boost::mpi::environment *env;
    };

}

#endif //PURPLE_CLUSTER_H
