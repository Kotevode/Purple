//
// Created by Mark on 15.05.17.
//

#ifndef PURPLE_DISTRIBUTION_INTERSECTON_H
#define PURPLE_DISTRIBUTION_INTERSECTON_H

#include <purple/purple.h>
#include <boost/shared_array.hpp>
#include "Job.h"

namespace Dirichlet {
    namespace Evaluation {
        struct Section {

            Section(): start(0), size(0), mesh(nullptr) {}

            Section(size_t start, size_t size, double * mesh) : start(
                    start), size(size), mesh(mesh) {}

            Section(size_t start, size_t size, const double *mesh):
                    start(start), size(size), mesh(new double[size]){
                memcpy(this->mesh.get(), mesh, size * sizeof(double));
            }

            Section(const double *mesh, const Job &a, const Job &b):
                    start(a.width * b.offset), size(a.width * (a.offset + a.height - b.offset)) {
                assert(a.offset < b.offset);
                assert(size > 0);
                this->mesh = boost::shared_array<double>(new double[size]);
                memcpy(this->mesh.get(), mesh + start, sizeof(double) * size);
            }

            size_t start, size;
            boost::shared_array<double> mesh;

            friend double *operator+=(double *mesh, const Section &s) {
                memcpy(mesh + s.start, s.mesh.get(), sizeof(double) * s.size);
                return mesh;
            }

            PURPLE_SERIALIZE() {
                ar & start;
                ar & size;
                if (Archive::is_loading::value) {
                    mesh = boost::shared_array<double>(new double[size]);
                }
                ar & boost::serialization::make_array<double>(mesh.get(), size);
            }

        };

    }
}

#endif //PURPLE_DISTRIBUTION_INTERSECTON_H
