//
// Created by Mark on 20.05.17.
//

#ifndef PURPLE_DISTRIBUTION_MESSAGES_H
#define PURPLE_DISTRIBUTION_MESSAGES_H

#include <vector>
#include "purple/Job.h"
#include <string>
#include <ctime>

namespace Purple {
    namespace Monitoring {
        namespace Messages {

            struct Message {
                Message():time(std::time(0)) {}

                std::time_t time;

            };

            struct ClusterCreated : public Message {};

            struct ProcessingStarted : public Message {
                struct JobInfo {
                    size_t weight;
                    size_t node;
                };

                template<typename __job_type>
                ProcessingStarted(const std::vector<__job_type> &jobs) {
                    info = std::vector<JobInfo>(jobs.size());
                    for_each(jobs.begin(), jobs.end(), [&](auto &j) {
                        this->info[j.get_index()] = { (size_t)j.get_weight(), (size_t)j.get_node_number() };
                    });
                }

                std::vector<JobInfo> info;

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

            };

            struct ProcessingDone : public Message {};

            struct ClusterFinalized : public Message {};

            struct LogMessage : public Message {
                LogMessage(std::string body) : body(body) {}

                std::string body;

            };

        }
    }
}

#endif //PURPLE_DISTRIBUTION_MESSAGES_H
