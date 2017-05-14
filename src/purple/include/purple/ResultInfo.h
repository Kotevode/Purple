//
// Created by Mark on 14.03.17.
//

#ifndef PURPLE_DISTRIBUTION_RESULT_INFO_H
#define PURPLE_DISTRIBUTION_RESULT_INFO_H

#include <boost/serialization/access.hpp>

namespace Purple {

    using namespace boost;

    template<class result_type>
    class ResultInfo {

    public:
        ResultInfo() {}

        ResultInfo(result_type result, int index) :
                result(result), index(index) {}

        ResultInfo(ResultInfo &&moving):
                result(std::move(moving.result)),index(moving.index) {}

        ResultInfo(const ResultInfo &copy):
                result(copy.result),index(copy.index) {}

        ResultInfo& operator =(const ResultInfo &copy) {
            this->result = copy.result;
            this->index = copy.index;
            return *this;
        }

        ResultInfo& operator =(ResultInfo &&moving) {
            this->result = std::move(moving.result);
            this->index = moving.index;
            return *this;
        }

        result_type get_result() const { return result; }

        int get_index() const { return index; }

    private:
        friend class serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & result;
            ar & index;
        }

        result_type result;
        int index;

    };
}

#endif //PURPLE_DISTRIBUTION_RESULT_INFO_H
