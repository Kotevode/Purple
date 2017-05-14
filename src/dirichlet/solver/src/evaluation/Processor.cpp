//
// Created by Mark on 25.04.17.
//

#include "Processor.h"
#include <limits>

using namespace std;

Dirichlet::Result Dirichlet::Evaluation::Processor::process(Dirichlet::Evaluation::Job &job) {

    // Get F slice
    const double *f_sliced = f.get() + job.offset * job.width;
    // Create U
    double *u = new double[job.height * job.width];
    memcpy(u, job.mesh.get(), sizeof(double) * job.height * job.width);

    // Iteration
    double max_error = 0.0;
    for (int i = 1; i < job.height - 1; i++)
        for (int j = 1; j < job.width - 1; j++) {
            double temp = u[i * job.width + j];
            u[i * job.width + j] =
                    0.25 * (u[(i + 1) * job.width + j] +
                            u[(i - 1) * job.width + j] +
                            u[i * job.width + j + 1] +
                            u[i * job.width + j - 1] -
                            f_sliced[i * job.width + j]);
            max_error = max(abs(temp - u[i * job.width + j]), max_error);
        }

    return Result(job.height, job.width, u, max_error, job.offset);
}
