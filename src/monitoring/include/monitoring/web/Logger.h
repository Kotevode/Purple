//
// Created by Mark on 23.05.17.
//

#ifndef PURPLE_DISTRIBUTION_WEB_LOGGER_H
#define PURPLE_DISTRIBUTION_WEB_LOGGER_H

#include <fstream>
#include <purple/monitoring/Logger.h>
#include <json/json.hpp>
#include <boost/mpi.hpp>
#include <thread>
#include <mutex>

namespace Monitoring {
    namespace Web {

        class Logger : public Purple::Monitoring::Logger {

        public:
            Logger(const std::string &fifo);

            virtual void send(const Purple::Monitoring::Messages::ClusterCreated &message) override;

            virtual void send(const Purple::Monitoring::Messages::ClusterFinalized &message) override;

            virtual void send(const Purple::Monitoring::Messages::JobStatusChanged &message) override;

            virtual void send(const Purple::Monitoring::Messages::ProcessingDone &message) override;

            virtual void send(const Purple::Monitoring::Messages::ProcessingStarted &message) override;

            virtual void send(const Purple::Monitoring::Messages::LogMessage &message) override;

            ~Logger();

        private:
            int size, rank, communication_tag;
            bool running;
            std::ofstream *out;
            boost::mpi::communicator communicator;
            std::thread communication_thread;
            std::mutex stream_mutex;

            void receive();

            void write(const nlohmann::json &json);

            void write(const std::string &dump);

        };

    }
}

#endif //PURPLE_DISTRIBUTION_WEB_LOGGER_H
