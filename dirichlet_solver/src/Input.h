//
// Created by Mark on 24.04.17.
//

#ifndef PURPLE_DISTRIBUTION_INPUT_H
#define PURPLE_DISTRIBUTION_INPUT_H

#include "DirichletJob.h"
#include <vector>
#include <iostream>

using namespace std;

struct Input {
    Input(double *mesh, int width, int height, const vector<DirichletJob, allocator<DirichletJob>> &jobs) : mesh(
            mesh), width(width), height(height), jobs(jobs) {}

    double *mesh;
    int width, height;
    vector<DirichletJob> jobs;
};


#endif //PURPLE_DISTRIBUTION_INPUT_H
