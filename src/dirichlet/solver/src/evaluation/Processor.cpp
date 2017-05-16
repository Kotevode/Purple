//
// Created by Mark on 25.04.17.
//

#include "Processor.h"
#include <limits>

using namespace std;

double Dirichlet::Evaluation::Processor::process(Dirichlet::Evaluation::Job &job) {

    // Get F slice
    const double *f_sliced = f.get() + job.offset * job.width;

    // Get U slice
    double *u_sliced = u.get() + job.offset * job.width;

    // Iteration
    double max_error = 0.0;
    for (int i = 1; i < job.height - 1; i++)
        for (int j = 1; j < job.width - 1; j++) {
            double temp = u_sliced[i * job.width + j];
            u_sliced[i * job.width + j] =
                    0.25 * (u_sliced[(i + 1) * job.width + j] +
                            u_sliced[(i - 1) * job.width + j] +
                            u_sliced[i * job.width + j + 1] +
                            u_sliced[i * job.width + j - 1] -
                            f_sliced[i * job.width + j]);
            max_error = max(abs(temp - u_sliced[i * job.width + j]), max_error);
        }

    return max_error;
}
