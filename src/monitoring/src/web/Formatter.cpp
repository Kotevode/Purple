//
// Created by Mark on 23.05.17.
//

#include "web/Formatter.h"
#include <algorithm>

using namespace nlohmann;
using namespace std;

template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::ClusterCreated>::to_s = "cluster_created";
template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::ProcessingStarted>::to_s = "processing_started";
template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::JobStatusChanged>::to_s = "job_status_changed";
template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::ProcessingDone>::to_s = "processing_done";
template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::ClusterFinalized>::to_s = "cluster_finalized";
template<>
const char *Monitoring::Web::MessageType<Purple::Monitoring::Messages::LogMessage>::to_s = "cluster_log_message_created";

nlohmann::json Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::ClusterCreated &message) const {
    auto j = prepare(message);
    j["content"]["size"] = communicator.size();
    return j;
}

nlohmann::json
Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::ProcessingStarted &message) const {
    auto j = prepare(message);
    auto info = json::array();
    for_each(message.info.begin(), message.info.end(), [&info](auto &job) {
        info.push_back(
                {
                        {"index",  job.index},
                        {"weight", job.weight},
                        {"node",   job.node}
                }
        );
    });
    j["content"]["info"] = info;
    return j;
}

nlohmann::json Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::JobStatusChanged &message) const {
    auto j = prepare(message);
    j["content"]["index"] = message.index;
    j["content"]["status"] = message.status;
    return j;
}

nlohmann::json Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::ProcessingDone &message) const {
    return prepare(message);
}

nlohmann::json Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::ClusterFinalized &message) const {
    return prepare(message);
}

nlohmann::json Monitoring::Web::Formatter::format(const Purple::Monitoring::Messages::LogMessage &message) const {
    auto j = prepare(message);
    j["content"]["body"] = message.body;
    return j;
}
