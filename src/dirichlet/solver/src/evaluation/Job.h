//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_JOB_H
#define PURPLE_DISTRIBUTION_EVALUATION_JOB_H

//
// Job.h
//

#include <purple/purple.h>
#include <ostream>
#include <algorithm>
#include <boost/shared_array.hpp>

namespace Dirichlet {
    namespace Evaluation {

        struct Job : public Purple::Job {

            Job() : offset(0), height(0), width(0) {}

            Job(size_t offset, size_t height, size_t width) :
                    offset(offset), height(height), width(width) {
            }

            // Метод, возвращающий трудоемкость задачи,
            // определяемой как площадь подобласти
            virtual int get_weight() const override { return height * width; }

            friend std::ostream &operator<<(std::ostream &os, const Job &job) {
                os << "offset: " << job.offset << " height: " << job.height;
                return os;
            }

            // Метод сериализации задачи, вызывающий метод сериалзиации
            // родительсткого класса
            PURPLE_SERIALIZE() {
                Purple::Job::serialize(ar, version);
                ar & offset;
                ar & height;
                ar & width;
            }

            // Смещение подобласти по высоте
            size_t offset;
            // Высота подобласти
            size_t height;
            // Ширина подобласти
            size_t width;

        };
    }
}


#endif //PURPLE_DISTRIBUTION_EVALUATION_JOB_H
