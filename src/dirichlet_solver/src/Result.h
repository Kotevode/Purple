//
// Created by Mark on 25.04.17.
//

#ifndef PURPLE_DISTRIBUTION_DIRICHLETRESULT_H
#define PURPLE_DISTRIBUTION_DIRICHLETRESULT_H

#include <purple/purple.h>
#include <boost/array.hpp>
#include <stdio.h>

using namespace boost::serialization;

namespace Dirichlet {

    struct Result {
        Result() {}

        Result(int height, int width, double *mesh, double error) : height(height), width(width), mesh(mesh),
                                                                    error(error) {}

        Result(const Result &r) : height(r.height), width(r.width), error(r.error) {
            mesh = (double *)malloc(sizeof(double) * width * height);
            memcpy(mesh, r.mesh, sizeof(double) * width * height);
        }

        Result(Result &&r) : height(r.height), width(r.width), error(r.error) {
            mesh = r.mesh;
            r.mesh = nullptr;
        }

        Result & operator=(const Result &r){
            height = r.height;
            width = r.width;
            error = r.error;
            mesh = (double *)malloc(sizeof(double) * width * height);
            memcpy(mesh, r.mesh, sizeof(double) * width * height);
            return *this;
        }

        Result & operator=(Result &&r){
            height = r.height;
            width = r.width;
            error = r.error;
            mesh = r.mesh;
            r.mesh = nullptr;
            return *this;
        }

        int height, width;
        double *mesh = nullptr;
        double error;

        PURPLE_SERIALIZE() {
            ar & height;
            ar & width;
            ar & error;
            if (Archive::is_loading::value) {
                assert(mesh == nullptr);
                mesh = (double *) malloc(sizeof(double) * height * width);
            }
            ar & make_array<double>(mesh, height * width);
            std::cout << "Serialized" << std::endl;
        }

        virtual ~Result() {
            if (mesh)
                free(mesh);
        }

    };

}

#endif //PURPLE_DISTRIBUTION_DIRICHLETRESULT_H