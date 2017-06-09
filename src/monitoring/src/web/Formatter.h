//
// Created by Mark on 23.05.17.
//

#ifndef PURPLE_DISTRIBUTION_WEB_FORMATTER_H
#define PURPLE_DISTRIBUTION_WEB_FORMATTER_H

#include <Formatter.h>
#include <json/json.hpp>
#include <boost/mpi.hpp>

namespace Monitoring {
    namespace Web {

        template<typename __message_type>
        struct MessageType {
            static const char * to_s;
        };

        class Formatter : public Monitoring::Formatter<nlohmann::json> {

        public:
            nlohmann::json format(const Purple::Monitoring::Messages::ClusterCreated &message) const override;

            nlohmann::json format(const Purple::Monitoring::Messages::ProcessingStarted &message) const override;

            nlohmann::json format(const Purple::Monitoring::Messages::JobStatusChanged &message) const override;

            nlohmann::json format(const Purple::Monitoring::Messages::ProcessingDone &message) const override;

            nlohmann::json format(const Purple::Monitoring::Messages::ClusterFinalized &message) const override;

            nlohmann::json format(const Purple::Monitoring::Messages::LogMessage &message) const override;

        private:
            template<typename __message_type>
            nlohmann::json prepare(const __message_type &message) const {
                nlohmann::json j;
                j["time"] = message.time;
                j["type"] = MessageType<__message_type>::to_s;
                return j;
            }

            boost::mpi::communicator communicator;

        };

    }
}

#endif //PURPLE_DISTRIBUTION_WEB_FORMATTER_H
