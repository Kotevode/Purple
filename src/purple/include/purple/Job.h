//
// Created by Mark on 12.03.17.
//

#ifndef PURPLE_JOB_H
#define PURPLE_JOB_H

#include <boost/serialization/access.hpp>

#define PURPLE_SERIALIZE()\
friend class boost::serialization::access;\
template <class Archive>\
void serialize(Archive &ar, const unsigned int version)

namespace Purple {

    class Job {

    public:
        virtual int get_weight() const = 0;

    };

}

#endif //PURPLE_JOB_H
