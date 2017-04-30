//
// Created by Mark on 25.04.17.
//

#include "Processor.h"
#include <limits>

using namespace std;

Dirichlet::Result Dirichlet::Evaluation::Processor::process(Dirichlet::Evaluation::Job &job) {
    double error = numeric_limits<double>().max();
    int height = job.height;

    // Slicing job targeted u part
    double *u_sliced = (double *) malloc(sizeof(double) * height * width);
    memcpy(u_sliced, u + job.offset * width, height * width);
    const double *f_sliced = f + job.offset * width;

    // Iterations
    while (error > job.error) {
        double max_error = 0.0;
        for (int i = 1; i < height - 1; i++)
            for (int j = 1; j < width - 1; j++) {
                double temp = u_sliced[i * width + j];
                u_sliced[i * width + j] =
                        0.25 * (u_sliced[(i + 1) * width + j] + u_sliced[(i - 1) * width + j] +
                                u_sliced[i * width + j + 1] + u_sliced[i * width + j - 1] -
                                f_sliced[i * width + j]);
                max_error = max(abs(temp - u_sliced[i * width + j]), max_error);
            }
        error = max_error;
    }

    return Result(height, width, u_sliced, error, job.offset);
}
