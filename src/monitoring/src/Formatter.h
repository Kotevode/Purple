//
// Created by Mark on 23.05.17.
//

#ifndef PURPLE_DISTRIBUTION_FORMATTER_H
#define PURPLE_DISTRIBUTION_FORMATTER_H

#include <purple/monitoring/messages/Messages.h>

namespace Monitoring {

    template<typename __out_type>
    class Formatter {

    public:
        
        virtual __out_type format(const Purple::Monitoring::Messages::ClusterCreated &message) const = 0;

        virtual __out_type format(const Purple::Monitoring::Messages::ProcessingStarted &message) const = 0;

        virtual __out_type format(const Purple::Monitoring::Messages::JobStatusChanged &message) const = 0;

        virtual __out_type format(const Purple::Monitoring::Messages::ProcessingDone &message) const = 0;

        virtual __out_type format(const Purple::Monitoring::Messages::ClusterFinalized &message) const = 0;

        virtual __out_type format(const Purple::Monitoring::Messages::LogMessage &message) const = 0;

    };

}

#endif //PURPLE_DISTRIBUTION_FORMATTER_H
