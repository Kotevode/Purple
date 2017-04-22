//
// Created by Mark on 14.03.17.
//

#ifndef PURPLE_DISTRIBUTION_JOB_INFO_H
#define PURPLE_DISTRIBUTION_JOB_INFO_H

#include <boost/serialization/access.hpp>

using namespace boost;

namespace Purple {

    class JobInfo {

    public:
        JobInfo() {}

        JobInfo(const Job &job, int index) :
                weight(job.get_weight()), index(index) {};

        int get_weight() const { return weight; }

        int get_index() const { return index; }

        int get_node_number() const { return node_number; }

        void set_node_number(int node_number) { this->node_number = node_number; }

        bool operator<(const JobInfo &rhs) const {
            return get_weight() < rhs.get_weight();
        }

        bool operator>(const JobInfo &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const JobInfo &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const JobInfo &rhs) const {
            return !(*this < rhs);
        }

    private:
        friend class serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & weight;
            ar & index;
            ar & node_number;
        }

        int weight;
        int index;
        int node_number;

    };

}

#endif //PURPLE_DISTRIBUTION_JOB_INFO_H
