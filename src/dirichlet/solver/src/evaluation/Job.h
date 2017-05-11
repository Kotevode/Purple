//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_EVALUATION_JOB_H
#define PURPLE_DISTRIBUTION_EVALUATION_JOB_H

#include <purple/purple.h>
#include <ostream>
#include <algorithm>
#include <boost/shared_array.hpp>

namespace Dirichlet {
    namespace Evaluation {

        class Job : public Purple::Job {

        public:
            Job() : offset(0), height(0), width(0), mesh(nullptr) {}

            Job(size_t offset, size_t height, size_t width, const double mesh[]) :
                    offset(offset), height(height), width(width), mesh(new double[height * width]) {
                memcpy(this->mesh.get(), mesh, sizeof(double) * width * height);
            }

            virtual int get_weight() const override { return height * width; }

            friend std::ostream &operator<<(std::ostream &os, const Job &job) {
                os << "offset: " << job.offset << " height: " << job.height;
                return os;
            }

        private:
            friend class Processor;

            PURPLE_SERIALIZE() {
                ar & offset;
                ar & height;
                ar & width;
                if (Archive::is_loading::value) {
                    assert(mesh.get() == nullptr);
                    mesh = boost::shared_array<double>(new double[height * width]);
                }
                ar & boost::serialization::make_array<double>(mesh.get(), height * width);
            }

            size_t offset, height, width;
            boost::shared_array<double> mesh;

        };
    }
}


#endif //PURPLE_DISTRIBUTION_EVALUATION_JOB_H
