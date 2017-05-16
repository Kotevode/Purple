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
#include "Processor.h"
#include "ResultInfo.h"
#include "efdist.h"
#include <numeric>

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

            // Mapping jobs

            if (communicator.rank() == 0)
                this->map(jobs);

            // Distributing jobs

            broadcast(communicator, jobs, 0);

            // Processing

            vector<ResultInfo<result_type> > partial_results;
            for_each(jobs.begin(), jobs.end(), [&processor, &partial_results, this](auto &job) {
                if (job.get_node_number() != this->communicator.rank())
                    return;
                partial_results.push_back(
                        ResultInfo<result_type>(processor.process(job), job.get_index())
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

        template<class job_type>
        void map(std::vector<job_type> &jobs) const {
            int i = 0;
            bool are_mapped = std::accumulate(jobs.begin(), jobs.end(), true, [&i](bool result, auto &j) {
                j.index = i++;
                return result & j.is_mapped();
            });
            if (are_mapped)
                return;
            ef_distribution(jobs.begin(), jobs.end(), (size_t) communicator.size());
            sort(jobs.begin(), jobs.end(), [](auto &a, auto &b) {
                return a.index < b.index;
            });
        }

        boost::mpi::communicator communicator;
        boost::mpi::environment *env;
    };

}

#endif //PURPLE_CLUSTER_H
