//
// Created by Mark on 24.04.17.
//

#include "StreamParser.h"

Input StreamParser::parse_input() {
    int height, width;
    double *mesh = nullptr;

    assert(!stream.eof());

    // Parsing mesh
    stream >> width;
    stream >> height;
    mesh = (double *) malloc(sizeof(double) * width * height);
    for (int i = 0; i < width * height; i++)
        stream >> mesh[i];

    // Parsing jobs
    int job_count;
    stream >> job_count;
    vector<DirichletJob> jobs;
    for (int i = 0; i < job_count; i++) {
        int offset;
        int height;
        stream >> offset;
        stream >> height;
        jobs.push_back(DirichletJob(offset, height));
    }

    return Input(mesh, width, height, jobs);
}