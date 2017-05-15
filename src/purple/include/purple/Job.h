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

        int get_index() const { return index; }

        int get_node_number() const { return node_number; }

        void set_node_number(int node_number) { this->node_number = node_number; }

        bool operator<(const Job &rhs) const {
            return get_weight() < rhs.get_weight();
        }

        bool operator>(const Job &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Job &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Job &rhs) const {
            return !(*this < rhs);
        }

        bool is_mapped() const { return node_number != -1; }

    protected:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & index;
            ar & node_number;
        }

    private:
        friend class Cluster;

        int index = 0;
        int node_number = -1;

    };

}

#endif //PURPLE_JOB_H
