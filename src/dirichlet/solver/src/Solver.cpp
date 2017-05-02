//
// Created by Mark on 28.04.17.
//

#include "Solver.h"

Dirichlet::Result Dirichlet::Solver::process(Dirichlet::Input &input) {
    auto results = evaluate(input.u, input.f, input.width, input.jobs);
    return conjugate(input.width, results);
}

Dirichlet::Result Dirichlet::Solver::conjugate(int width, vector<Dirichlet::Result> results) {
    Conjugation::Processor p(width);
    int count = results.size();
    while (count != 1) {
        vector<Conjugation::Job> jobs;
        cluster->as_master([&] {
            auto res_it = results.begin();
            for (int i = 0; i < count / 2 * 2; i += 2)
                jobs.push_back(Conjugation::Job(*res_it++, *res_it++));
            if (res_it != results.end())
                jobs.push_back(Conjugation::Job(*res_it));
        });
        results = cluster->process(jobs, p);
        count = results.size();
    }
    return results.front();
}

vector<Dirichlet::Result, allocator<Dirichlet::Result>>
Dirichlet::Solver::evaluate(const double *u, const double *f, int width, vector<Evaluation::Job> &jobs) {
    Evaluation::Processor p(u, f, width);
    return cluster->process(jobs, p);
}