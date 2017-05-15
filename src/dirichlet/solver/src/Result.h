//
// Created by Mark on 25.04.17.
//

#ifndef PURPLE_DISTRIBUTION_DIRICHLETRESULT_H
#define PURPLE_DISTRIBUTION_DIRICHLETRESULT_H

#include <purple/purple.h>
#include <boost/shared_array.hpp>
#include <algorithm>
#include <stdio.h>

namespace Dirichlet {

    struct Result {
        Result() : height(0), width(0), error(-1), offset(0) {}

        Result(size_t height, size_t width, const double *mesh, double error, size_t offset) :
                height(height), width(width), mesh(new double[height * width]),
                error(error), offset(offset) {
            memcpy(this->mesh.get(), mesh, sizeof(double) * width * height);
        }

        size_t height, width;
        size_t offset;
        double error;
        boost::shared_array<double> mesh;

        PURPLE_SERIALIZE() {
            ar & height;
            ar & width;
            ar & offset;
            ar & error;
            if (Archive::is_loading::value) {
                mesh = boost::shared_array<double>(new double[height * width]);
            }
            ar & boost::serialization::make_array<double>(mesh.get(), height * width);
        }

    };

}

#endif //PURPLE_DISTRIBUTION_DIRICHLETRESULT_H
