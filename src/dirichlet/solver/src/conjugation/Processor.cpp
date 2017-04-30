//
// Created by Mark on 28.04.17.
//

#include "Processor.h"
#include <algorithm>

using namespace std;

Dirichlet::Result Dirichlet::Conjugation::Processor::process(Dirichlet::Conjugation::Job &job) {
    if (job.b.height == 0)
        return Result(job.a);
    // Height of difference between a.mesh and b.mesh
    int diff = job.a.height - (job.b.offset - job.a.offset);

    // Height of result mesh
    int height = job.b.offset - job.a.offset + job.b.height;
    double * u_result = (double *)malloc(sizeof(double) * width * height);
    memcpy(u_result, job.a.mesh, sizeof(double) * width * (job.a.height - diff));
    memcpy(u_result + width * job.a.height, job.b.mesh + width * diff, sizeof(double) * width * (job.b.height - diff));

    // Conjugation of difference
    double *diff_start = u_result + width * (job.a.height - diff);
    double *diff_start_a = job.a.mesh + width * (job.a.height - diff);
    double *diff_start_b = job.b.mesh;
    for (int i = 0; i < width * diff; i++)
        diff_start[i] = (diff_start_a[i] + diff_start_b[i]) / 2;

    return Result(height, width, u_result, max(job.a.error, job.b.error), job.a.offset);
}
