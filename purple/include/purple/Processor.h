//
// Created by Mark on 12.03.17.
//

#ifndef PURPLE_PROCESSOR_H
#define PURPLE_PROCESSOR_H

#include "Job.h"

namespace Purple {

    template<class job_type, class result_type>
    class Processor {

    public:
        virtual result_type process(job_type &job) = 0;

    };

}

#endif //PURPLE_PROCESSOR_H
