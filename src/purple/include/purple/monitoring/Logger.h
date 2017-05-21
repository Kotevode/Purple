//
// Created by Mark on 20.05.17.
//

#ifndef PURPLE_DISTRIBUTION_LOGGER_H
#define PURPLE_DISTRIBUTION_LOGGER_H

#include "messages/Messages.h"

namespace Purple {
    namespace Monitoring {

        class Logger {

        public:
            virtual void send(const Messages::ClusterCreated &message) = 0;

            virtual void send(const Messages::ClusterFinalized &message) = 0;

            virtual void send(const Messages::JobStatusChanged &message) = 0;

            virtual void send(const Messages::ProcessingDone &message) = 0;

            virtual void send(const Messages::ProcessingStarted &message) = 0;

            virtual void send(const Messages::LogMessage &message) = 0;

            virtual ~Logger() {}

        };

    }
}

#endif //PURPLE_DISTRIBUTION_LOGGER_H
