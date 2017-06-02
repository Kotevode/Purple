//
// Created by Mark on 28.04.17.
//

#include "Solver.h"
#include <float.h>
#include "evaluation/Section.h"
#include <boost/mpi/collectives.hpp>

using namespace Dirichlet;
using namespace Evaluation;
using namespace std;
using namespace boost::mpi;
using namespace boost;

shared_array<double> Dirichlet::Solver::process(Dirichlet::Input &input, double error) {
    Processor p(input.f, input.u);
    auto jobs = input.jobs;
    vector<double> results;
    bool is_done = false;
    double max_error = 0;

    do {

        // Processing jobs

        results = cluster->process(jobs, p);
        cluster->as_master([&is_done, error, &results, &max_error] {
            max_error = *max_element(results.begin(), results.end());
            cout << max_error << endl;
            is_done = max_error < error;
        });

        // Break loop if accuracy has been achieved

        broadcast(cluster->get_communicator(), is_done, 0);
        if (is_done) {
            break;
        }

        // Swapping intersections of subfields

        this->swap_intersections(jobs, p.u.get());
    } while (true);

    // Combining subfields
    return combine(jobs, p.u.get());
}

void Dirichlet::Solver::swap_intersections(vector<Evaluation::Job> &jobs, double *mesh) {
    list<Section> sections;
    sections.splice(sections.end(), swap_intersections(jobs.begin(), jobs.end(), mesh));
    sections.splice(sections.end(), swap_intersections(jobs.rbegin(), jobs.rend(), mesh));
    for_each(sections.begin(), sections.end(), [mesh](auto &s){
        mesh += s;
    });
}

shared_array<double>
Dirichlet::Solver::combine(vector<Job> &jobs, const double *mesh) {
    auto communicator = cluster->get_communicator();
    for_each(jobs.begin(), jobs.end(), [mesh, &communicator](auto &j) {
        if (j.get_node_number() != communicator.rank() || j.get_node_number() == 0)
            return;
        communicator.isend(0, 1, Section(j.offset * j.width, j.width * j.height, mesh + j.offset * j.width));
    });
    size_t size = jobs.front().width * (jobs.back().offset + jobs.back().height);
    shared_array<double> result(new double[size]);
    memcpy(result.get(), mesh, size * sizeof(double));
    cluster->as_master([&]{
        for_each(jobs.begin(), jobs.end(), [&communicator, &result](auto &j) {
           if (j.get_node_number() != 0) {
               Section s;
               communicator.recv(j.get_node_number(), 1, s);
               result.get() += s;
           }
        });
    });
    return result;
}