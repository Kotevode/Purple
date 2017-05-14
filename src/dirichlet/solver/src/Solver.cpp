//
// Created by Mark on 28.04.17.
//

#include "Solver.h"
#include <float.h>

using namespace Dirichlet;
using namespace Evaluation;
using namespace boost::mpi;

Dirichlet::Result Dirichlet::Solver::process(Dirichlet::Input &input, double error) {
    Processor p(input.f);
    auto jobs = input.jobs;
    vector<Result> results;
    bool is_done = false;
    double max_error = 0;

    do {

        // Processing jobs

        results = cluster->process(jobs, p);
        cluster->as_master([&is_done, error, &results, &max_error] {
            max_error = max_element(results.begin(), results.end(), [](auto &a, auto &b) {
                return a.error < b.error;
            })->error;
            cout << max_error << endl;
            is_done = max_error < error;
        });

        // Break loop if accuracy has been achieved

        broadcast(cluster->get_communicator(), is_done, 0);
        if (is_done) {
            break;
        }

        // Swapping intersections of subfields

        jobs.clear();
        cluster->as_master([&jobs, &results, this] {
            for_each(results.begin(), results.end(), [&jobs](auto &r) {
                jobs.push_back(Job(r.offset, r.height, r.width, r.mesh.get()));
            });
            this->swap_intersections(jobs);
        });
    } while (true);

    // Combining sufields

    Result result;
    cluster->as_master([&results, &result, &input, max_error, this](){
        result = this->combine(results, input.height, max_error);
    });
    return result;
}

void Dirichlet::Solver::swap_intersections(vector<Evaluation::Job> &jobs) {
    auto begin = jobs.begin();
    auto end = jobs.end();
    while (begin != end - 1)
        swap_intersections(*begin++, *begin);
}

void Dirichlet::Solver::swap_intersections(Evaluation::Job &a, Evaluation::Job &b) {
    assert(b.offset > a.offset);
    assert(b.offset < a.offset + a.height);
    swap_ranges(
            a.mesh.get() + (b.offset - a.offset) * a.width,
            a.mesh.get() + a.width * a.height,
            b.mesh.get()
    );
}

Result
Dirichlet::Solver::combine(vector<Result> &results, size_t height, double error) {
    size_t width = results.begin()->width;
    double *result = new double[height * width];
    for_each(results.begin(), results.end(), [&result](auto &r) {
       memcpy(result + r.width * r.offset, r.mesh.get(), r.width * r.height * sizeof(double));
    });
    return Result(height, width, result, error, 0);
}