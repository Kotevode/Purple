//
// Created by Mark on 20.05.17.
//

#ifndef PURPLE_DISTRIBUTION_MESSAGES_H
#define PURPLE_DISTRIBUTION_MESSAGES_H

#include <map>
#include "../../Job.h"

namespace Purple {
    namespace Monitoring {
        namespace Messages {

            struct Message {};

            struct ClusterCreated : public Message {
                ClusterCreated(size_t size) : size(size) {}

                size_t size;
            };

            struct ProcessingStarted : public Message {

                template<typename __job_type>
                ProcessingStarted(const vector<__job_type> &jobs) {
                    for_each(jobs.begin(), jobs.end(), [&](auto &j) {
                        this->jobs[j.get_index()] = j.get_weight();
                    });
                }

                std::map<size_t, size_t> jobs;

                PURPLE_SERIALIZE () {
                    ar & jobs;
                }
            };

            struct JobsDistributed : public Message {

                template<typename  __job_type>
                JobsDistributed(const vector<__job_type> &jobs) {
                    for_each(jobs.begin(), jobs.end(), [&](auto &j) {
                        this->mapping[j.get_index()] = j.get_weight();
                    });
                }

                std::map<size_t, size_t> mapping;

                PURPLE_SERIALIZE () {
                    ar & mapping;
                }
            };

            struct JobStatusChanged : public Message {
                enum JobStatus {
                    WAITING,
                    RUNNING,
                    DONE,
                    ERROR
                };

                JobStatusChanged(size_t index, JobStatus status = WAITING) : index(index), status(status) {}

                size_t index;
                JobStatus status;

                PURPLE_SERIALIZE () {
                    ar & index;
                    ar & status;
                }
            };

            struct ProcessingDone : public Message {
            };

            struct ClusterFinalized : public Message {
            };

            struct LogMessage : public Message {
                LogMessage(string message) : message(message) {}

                string message;

                PURPLE_SERIALIZE () {
                    ar & message;
                }

            };

        }
    }
}

#endif //PURPLE_DISTRIBUTION_MESSAGES_H
