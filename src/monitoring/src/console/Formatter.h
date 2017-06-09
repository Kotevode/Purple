//
// Created by Mark on 23.05.17.
//

#ifndef PURPLE_DISTRIBUTION_CONSOLE_FORMATTER_H
#define PURPLE_DISTRIBUTION_CONSOLE_FORMATTER_H

#include <Formatter.h>
#include <string>
#include <boost/mpi.hpp>

namespace Monitoring {
    namespace Console {

        class Formatter : public Monitoring::Formatter<std::string> {

        public:
            std::string format(const Purple::Monitoring::Messages::ClusterCreated &message) const override;

            std::string format(const Purple::Monitoring::Messages::ProcessingStarted &message) const override;

            std::string format(const Purple::Monitoring::Messages::JobStatusChanged &message) const override;

            std::string format(const Purple::Monitoring::Messages::ProcessingDone &message) const override;

            std::string format(const Purple::Monitoring::Messages::ClusterFinalized &message) const override;

            std::string format(const Purple::Monitoring::Messages::LogMessage &message) const override;

        private:
            boost::mpi::communicator communicator;

        };

    }
}

#endif //PURPLE_DISTRIBUTION_CONSOLE_FORMATTER_H
