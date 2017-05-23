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
#include "monitoring/Logger.h"

namespace Purple {

    class Cluster {

    public:
        Cluster(Monitoring::Logger *logger = nullptr) : logger(logger) {
            this->log(Monitoring::Messages::ClusterCreated());
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
            this->log(Monitoring::Messages::ProcessingStarted(jobs));

            // Distributing jobs

            broadcast(communicator, jobs, 0);

            // Processing

            vector<ResultInfo<result_type> > partial_results;
            for_each(jobs.begin(), jobs.end(), [&processor, &partial_results, this](auto &job) {
                if (job.get_node_number() != this->communicator.rank())
                    return;
                this->log(Monitoring::Messages::JobStatusChanged(
                        job.get_index(),
                        Monitoring::Messages::JobStatusChanged::RUNNING
                ));
                partial_results.push_back(
                        ResultInfo<result_type>(processor.process(job), job.get_index())
                );
                this->log(Monitoring::Messages::JobStatusChanged(
                        job.get_index(),
                        Monitoring::Messages::JobStatusChanged::DONE
                ));
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

            this->log(Monitoring::Messages::ProcessingDone());

            return results;
        }

        template<class job_type>
        void process(vector<job_type> &jobs, Processor<job_type, void> &processor) const {

            // Mapping jobs

            if (communicator.rank() == 0)
                this->map(jobs);
            this->log(Monitoring::Messages::ProcessingStarted(jobs));

            // Distributing jobs

            broadcast(communicator, jobs, 0);

            // Processing

            for_each(jobs.begin(), jobs.end(), [&processor, this](auto &job) {
                if (job.get_node_number() != this->communicator.rank())
                    return;
                this->log(Monitoring::Messages::JobStatusChanged(
                        job.get_index(),
                        Monitoring::Messages::JobStatusChanged::RUNNING
                ));
                processor.process(job);
                this->log(Monitoring::Messages::JobStatusChanged(
                        job.get_index(),
                        Monitoring::Messages::JobStatusChanged::DONE
                ));
            });
            MPI_Barrier(communicator);

            this->log(Monitoring::Messages::ProcessingDone());
        }

        const mpi::communicator &get_communicator() const {
            return this->communicator;
        }

        template<typename __message_type>
        void log(const __message_type &message) const {
            if (logger)
                logger->send(message);
        }

        ~Cluster() {
            log(Monitoring::Messages::ClusterFinalized());
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
        }

        boost::mpi::communicator communicator;
        boost::shared_ptr<Monitoring::Logger> logger;

    };

}

#endif //PURPLE_CLUSTER_H
