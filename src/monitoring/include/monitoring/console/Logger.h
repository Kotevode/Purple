//
// Created by Mark on 21.05.17.
//

#ifndef PURPLE_DISTRIBUTION_CONSOLE_LOGGER_H
#define PURPLE_DISTRIBUTION_CONSOLE_LOGGER_H

#include <purple/monitoring/Logger.h>
#include <boost/mpi.hpp>
#include <thread>

namespace Monitoring {
    namespace Console {

        class Logger : public Purple::Monitoring::Logger {
        public:
            Logger();

            virtual void send(const Purple::Monitoring::Messages::ClusterCreated &message) override;

            virtual void send(const Purple::Monitoring::Messages::ClusterFinalized &message) override;

            virtual void send(const Purple::Monitoring::Messages::JobStatusChanged &message) override;

            virtual void send(const Purple::Monitoring::Messages::ProcessingDone &message) override;

            virtual void send(const Purple::Monitoring::Messages::ProcessingStarted &message) override;

            virtual void send(const Purple::Monitoring::Messages::LogMessage &message) override;

            virtual ~Logger();

        private:
            int size, rank, communication_tag;
            bool running;
            boost::mpi::communicator communicator;
            std::thread communication_thread;

            void print(const std::string &s);

            void receive();
        };

    }
}

#endif //PURPLE_DISTRIBUTION_CONSOLE_LOGGER_H
