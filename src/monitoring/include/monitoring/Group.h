//
// Created by Mark on 21.05.17.
//

#ifndef PURPLE_DISTRIBUTION_GROUP_H
#define PURPLE_DISTRIBUTION_GROUP_H

#include <purple/monitoring/Logger.h>
#include <list>

namespace Monitoring {

    class Group {

    public:
        Group(const std::list<Purple::Monitoring::Logger *, std::allocator<Purple::Monitoring::Logger *>> &loggers)
                : loggers(loggers) {}

        template<typename __message_type>
        void send(const __message_type &message) const {
            for_each(loggers.begin(), loggers.end(), [&message](auto &m) {
                m.send(message);
            });
        }

    private:
        std::list<Purple::Monitoring::Logger *> loggers;

    };

}

#endif //PURPLE_DISTRIBUTION_GROUP_H
